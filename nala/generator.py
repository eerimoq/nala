"""Module in charge of generating mocks."""

import os
import re
from copy import deepcopy

from jinja2 import Environment
from jinja2 import PackageLoader
from pycparser import c_ast
from pycparser.c_generator import CGenerator

from .inspect import PRIMITIVE_TYPES
from . import __version__


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

HEADER_FILE = 'nala_mocks.h'
SOURCE_FILE = 'nala_mocks.c'
LINKER_FILE = 'nala_mocks.ldflags'


def is_ellipsis(param):
    return isinstance(param, c_ast.EllipsisParam)


def decl(name, type):
    return c_ast.Decl(name, [], [], [], type, None, None)


def function_ptr_decl(name, return_type, parameters):
    return decl(
        name,
        c_ast.PtrDecl([], c_ast.FuncDecl(c_ast.ParamList(parameters), return_type))
    )


def bool_param(name):
    return decl(name, c_ast.TypeDecl(name, [], c_ast.IdentifierType(['bool'])))


def set_member(name):
    return decl(name,
                c_ast.TypeDecl(name,
                               [],
                               c_ast.IdentifierType(['struct nala_set_param'])))


def in_assert_member(name, param_actual, param_expected):
    name = f'{name}_in_assert'

    return function_ptr_decl(
        name,
        void_type(name),
        [
            param_actual,
            param_expected,
            decl('size',
                 c_ast.TypeDecl('size', [], c_ast.IdentifierType(['size_t'])))
        ])


def out_copy_member(name, param_dst, param_src):
    name = f'{name}_out_copy'

    return function_ptr_decl(
        name,
        void_type(name),
        [
            param_dst,
            param_src,
            decl('size',
                 c_ast.TypeDecl('size', [], c_ast.IdentifierType(['size_t'])))
        ])


def va_list_param(name):
    return decl(None,
                c_ast.TypeDecl(name, [], c_ast.IdentifierType(['va_list'])))


def is_variadic_func(params):
    if params:
        return is_ellipsis(params[-1])
    else:
        return False


def void_type(name):
    return c_ast.TypeDecl(name, [], c_ast.IdentifierType(['void']))


def int_type(name):
    return c_ast.TypeDecl(name, [], c_ast.IdentifierType(['int']))


def rename_return_type(return_type, name):
    return_type = deepcopy(return_type)
    type_decl = return_type

    while not isinstance(type_decl, c_ast.TypeDecl):
        type_decl = type_decl.type

    type_decl.declname = name

    return return_type


def create_implementation_params(params):
    return [
        va_list_param('__nala_va_list')
        if is_ellipsis(param)
        else param
        for param in params
    ]


def get_guard_name(filename):
    slug = re.sub(r'[^a-zA-Z0-9]', '_', os.path.normpath(os.path.relpath(filename)))

    return re.sub(r'_+', '_', slug).upper().strip('_')


def generate_includes(includes, directory):
    include_lines = []

    for path, system in includes:
        if system:
            include_lines.append(f'#include <{path}>')
        else:
            include_lines.append(f'#include "{os.path.relpath(path, directory)}"')

    return '\n'.join(include_lines)


def read_nala_c():
    path = os.path.join(SCRIPT_DIR, 'templates', 'nala.c')

    with open(path, 'r') as fin:
        return fin.read()


class StructAssert:

    def __init__(self, struct, struct_names, struct_typedef_names):
        self.name = struct[0]
        self.assert_eq_members = []
        self.assert_eq_cast_members = []
        self.assert_array_eq_members = []
        self.assert_struct_members = []

        for member in struct[1]:
            if member[0] == 'assert-eq':
                self.assert_eq_members.append(member[1])
            elif member[0] == 'assert-eq-bit-field':
                self.assert_eq_cast_members.append(member[1:])
            elif member[0] == 'assert-array-eq':
                self.assert_array_eq_members.append(member[1])
            elif member[0] == 'assert-struct':
                if member[2] in struct_names:
                    self.assert_struct_members.append(member[1:])


class StructTypedefAssert:

    def __init__(self, typedef, struct_names, struct_typedef_names):
        self.name = typedef[0]
        self.assert_eq_members = []
        self.assert_eq_cast_members = []
        self.assert_array_eq_members = []
        self.assert_struct_members = []

        for member in typedef[1]:
            if member[0] == 'assert-eq':
                self.assert_eq_members.append(member[1])
            elif member[0] == 'assert-eq-bit-field':
                self.assert_eq_cast_members.append(member[1:])
            elif member[0] == 'assert-array-eq':
                self.assert_array_eq_members.append(member[1])
            elif member[0] == 'assert-struct':
                if member[2] in struct_names:
                    self.assert_struct_members.append(member[1:])


class FunctionMock:

    DECL_MARKER = "// NALA_DECLARATION"
    IMPL_MARKER = "// NALA_IMPLEMENTATION"

    def __init__(self,
                 function,
                 has_implementation,
                 real_variadic_function,
                 generator):
        self.function = function
        self.func_name = function.name
        self.real_variadic_function = real_variadic_function
        self.struct_names = generator.struct_names
        self.struct_typedef_names = generator.struct_typedef_names

        self.wrapped_func = f'__wrap_{self.func_name}'
        self.real_func = f'__real_{self.func_name}'

        self.state_name = f'nala_mock_{self.func_name}'

        self.generator = generator
        self.func_decl = self.function.declaration.type
        self.func_params = self.func_decl.args.params if self.func_decl.args else []
        self.assign_names_to_unnamed_params(self.func_params)
        self.is_variadic_func = is_variadic_func(self.func_params)

        if has_implementation is not None:
            self.has_implementation = has_implementation
        elif self.is_variadic_func:
            if real_variadic_function:
                self.has_implementation = True
            else:
                self.has_implementation = False
        else:
            self.has_implementation = True

        self.params_struct = []

        for param in self.func_params:
            if is_ellipsis(param) or not param.name:
                continue

            if isinstance(param.type, c_ast.ArrayDecl):
                param = decl(param.name, c_ast.PtrDecl([], param.type.type))

            if 'const' in param.type.quals:
                param = deepcopy(param)
                param.type.quals = [qual
                                    for qual in param.type.quals
                                    if qual != 'const']

            self.params_struct.append(param)

        self.forward_args = ', '.join(param.name for param in self.params_struct)

        if self.is_variadic_func:
            self.params_struct.append(decl(
                None,
                c_ast.PtrDecl([],
                              c_ast.TypeDecl('vafmt_p',
                                             ['const'],
                                             c_ast.IdentifierType(['char'])))))
            self.forward_args += ', nala_vl'

        # -Wpedantic warns on empty structs.
        if not self.params_struct:
            self.params_struct = [
                decl('dummy',
                     c_ast.TypeDecl('dummy', [], c_ast.IdentifierType(['int'])))
            ]

        return_type = self.func_decl.type
        self.return_value = (
            None
            if isinstance(return_type, c_ast.TypeDecl)
            and isinstance(return_type.type, c_ast.IdentifierType)
            and return_type.type.names[0] == 'void'
            else 'return_value')

        if self.is_variadic_func:
            self.va_list_start_arg_name = self.func_params[-2].name
        else:
            self.va_list_start_arg_name = None

        self.return_value_decl = decl(
            self.return_value,
            rename_return_type(return_type, self.return_value))
        mock_params = self.create_mock_params()
        self.implementation_decl = function_ptr_decl(
            'implementation',
            rename_return_type(return_type, 'implementation'),
            create_implementation_params(self.func_params))
        self.mock_func = self.void_function_decl(f'{self.func_name}_mock',
                                                 mock_params)
        self.mock_once_func = self.int_function_decl(
            f'{self.func_name}_mock_once',
            mock_params)
        self.set_errno_func = self.void_function_decl(
            f'{self.func_name}_mock_set_errno',
            [decl(
                'errno_value',
                c_ast.TypeDecl('errno_value', [], c_ast.IdentifierType(['int'])),
            )])
        self.callback_decl = function_ptr_decl(
            'callback',
            void_type('callback'),
            create_implementation_params(self.func_params))
        self.variadic_func_real_wrapper_decl = c_ast.FuncDecl(
            c_ast.ParamList(create_implementation_params(self.func_params)),
            c_ast.TypeDecl(
                f'{self.func_name}_mock_va_arg_real',
                [],
                return_type))
        self.default_variadic_func_real_wrapper_decl = c_ast.FuncDecl(
            c_ast.ParamList(create_implementation_params(self.func_params)),
            c_ast.TypeDecl(
                f'nala_v{self.func_name}',
                [],
                return_type))
        self.real_decl = self.rename_function(self.real_func)
        self.wrapped_decl = self.rename_function(self.wrapped_func)
        self.instance_members = []
        self.set_params = []
        self.char_pointer_params = []
        self.pointer_params = []
        self.non_pointer_params = []
        self.ignore_params = []

        for param in self.func_params:
            if is_ellipsis(param):
                continue
            elif self.is_struct(param):
                continue
            elif self.is_union(param):
                continue
            elif self.is_va_list(param):
                continue

            if not param.name:
                continue

            self.instance_members.append(bool_param(f'ignore_{param.name}_in'))

            if self.is_char_pointer(param):
                self.char_pointer_params.append(param)
            elif self.is_pointer(param):
                self.pointer_params.append(param)
            else:
                self.non_pointer_params.append(param)

            if self.is_char_pointer_or_non_pointer(param):
                self.ignore_params.append(param.name)

            if not self.is_pointer(param):
                continue

            param_buf = self.rename_param(param, 'buf_p')
            param_actual = self.rename_param(param, 'actual_p')
            param_expected = self.rename_param(param, 'expected_p')
            param_dst = self.rename_param(param, 'dst_p')
            param_src = self.rename_param(param, 'src_p')
            self.instance_members.append(set_member(f'{param.name}_in'))
            self.instance_members.append(in_assert_member(param.name,
                                                          param_actual,
                                                          param_expected))
            self.instance_members.append(set_member(f'{param.name}_out'))
            self.instance_members.append(out_copy_member(param.name,
                                                         param_dst,
                                                         param_src))
            self.set_params.append((param,
                                    param_buf,
                                    param_actual,
                                    param_expected,
                                    param_dst,
                                    param_src,
                                    self.find_check_function(param)))

    def assign_names_to_unnamed_params(self, params):
        for i, param in enumerate(params):
            if not isinstance(param, c_ast.Typename):
                continue

            if self.is_void(param):
                continue

            name = f'nala_{i}'
            param.name = name

            while not isinstance(param, c_ast.TypeDecl):
                param = param.type

            param.declname = name

    def find_check_function(self, param):
        if self.is_pointer_pointer(param):
            return f'nala_mock_assert_pointer'
        elif self.is_char_pointer(param):
            return 'nala_mock_assert_in_string'
        elif self.is_primitive_type_pointer(param):
            return f'nala_mock_assert_{"_".join(param.type.type.type.names)}'
        elif self.is_struct_typedef_pointer(param):
            param = self.lookup_typedef(param.type.type.type.names[0])

            if param.name in self.struct_typedef_names:
                return f'nala_mock_assert_typedef_struct_{param.name}'
        elif self.is_struct_pointer(param):
            if param.type.type.type.name in self.struct_names:
                return f'nala_mock_assert_struct_{param.type.type.type.name}'

        return 'nala_assert_memory'

    def is_primitive_type_pointer(self, param):
        if not isinstance(param.type, c_ast.PtrDecl):
            return False

        try:
            name = ' '.join(param.type.type.type.names)

            if name not in PRIMITIVE_TYPES:
                return False
        except AttributeError:
            return False

        return True

    def is_pointer_pointer(self, param):
        if not isinstance(param.type, c_ast.PtrDecl):
            return False

        if not isinstance(param.type.type, c_ast.PtrDecl):
            return False

        return True

    def void_function_decl(self, name, parameters):
        if not parameters:
            parameters = [void_type('')]

        return c_ast.FuncDecl(c_ast.ParamList(parameters),
                              void_type(name))

    def int_function_decl(self, name, parameters):
        if not parameters:
            parameters = [void_type('')]

        return c_ast.FuncDecl(c_ast.ParamList(parameters),
                              int_type(name))

    def rename_function(self, name):
        return decl(
            name,
            c_ast.FuncDecl(self.func_decl.args,
                           rename_return_type(self.func_decl.type, name)))

    def rename_param(self, param, name):
        param = deepcopy(param)
        param.name = name
        param_next = param

        while not isinstance(param_next, c_ast.TypeDecl):
            param_next = param_next.type

        param_next.declname = name

        return param

    def is_va_list(self, param):
        try:
            if param.type.type.names[0] == '__builtin_va_list':
                return True
        except AttributeError:
            pass

        try:
            name = param.type.type.names[0]

            return self.is_va_list(self.lookup_typedef(name))
        except AttributeError:
            pass

        return False

    def is_char_pointer(self, param):
        if is_ellipsis(param):
            return False
        elif isinstance(param.type, c_ast.PtrDecl):
            if isinstance(param.type.type, (c_ast.FuncDecl, c_ast.PtrDecl)):
                return False
            elif isinstance(param.type.type.type, c_ast.Struct):
                return False
            elif param.type.type.type.names[0] == 'char':
                return True
            else:
                return False
        else:
            return False

    def is_char_pointer_or_non_pointer(self, param):
        if is_ellipsis(param):
            return False
        elif self.is_char_pointer(param):
            return True
        elif self.is_pointer(param):
            return False
        else:
            return True

    def is_void(self, param):
        if is_ellipsis(param):
            return False
        elif self.is_pointer(param):
            return False
        elif self.is_enum(param):
            return False
        elif self.is_struct(param):
            return False
        elif self.is_union(param):
            return False
        else:
            return param.type.type.names[0] == 'void'

    def is_pointer(self, param):
        if isinstance(param.type, (c_ast.PtrDecl, c_ast.ArrayDecl)):
            return True

        try:
            name = param.type.type.names[0]

            return self.is_pointer(self.lookup_typedef(name))
        except AttributeError:
            pass

        return False

    def is_enum(self, param):
        return isinstance(param.type.type, c_ast.Enum)

    def is_struct_pointer(self, param):
        if not self.is_pointer(param):
            return False

        if not self.is_struct(param.type):
            return False

        return True

    def is_struct_typedef_pointer(self, param):
        if not self.is_pointer(param):
            return False

        if not isinstance(param.type.type, c_ast.TypeDecl):
            return False

        if not isinstance(param.type.type.type, c_ast.IdentifierType):
            return False

        param = self.lookup_typedef(param.type.type.type.names[0])

        if param is None:
            return False

        if not isinstance(param.type.type, c_ast.Struct):
            return False

        return True

    def is_struct(self, param):
        try:
            if isinstance(param.type.type, c_ast.Struct):
                return True
        except AttributeError:
            pass

        try:
            name = param.type.type.names[0]

            return self.is_struct(self.lookup_typedef(name))
        except AttributeError:
            pass

        return False

    def is_union(self, param):
        try:
            if isinstance(param.type.type, c_ast.Union):
                return True
        except AttributeError:
            pass

        try:
            name = param.type.type.names[0]

            return self.is_union(self.lookup_typedef(name))
        except AttributeError:
            pass

        return False

    def lookup_typedef(self, name):
        return self.generator.lookup_typedef(name)

    def create_mock_params(self):
        once_params = []
        variable_arguments_params = []

        for param in self.func_params:
            if self.is_void(param):
                continue
            elif self.is_struct(param):
                continue
            elif self.is_union(param):
                continue
            elif self.is_va_list(param):
                continue
            elif self.is_char_pointer_or_non_pointer(param):
                once_params.append(param)
            elif is_ellipsis(param):
                variable_arguments_params.append(decl(
                    None,
                    c_ast.PtrDecl([],
                                  c_ast.TypeDecl('vafmt_p',
                                                 ['const'],
                                                 c_ast.IdentifierType(['char'])))))
                variable_arguments_params.append(param)

        if not self.is_void(self.return_value_decl):
            once_params.append(self.return_value_decl)

        once_params += variable_arguments_params

        return once_params


class FileGenerator:

    def __init__(self, parser=None):
        self.code_generator = CGenerator()

        self.jinja_env = Environment(
            loader=PackageLoader('nala', 'templates'),
            trim_blocks=True,
            lstrip_blocks=True)
        self.jinja_env.filters['render'] = self.code_generator.visit

        self.header_template = self.jinja_env.get_template(f'{HEADER_FILE}.jinja2')
        self.source_template = self.jinja_env.get_template(f'{SOURCE_FILE}.jinja2')

        self.mocks = []
        self.includes = []
        self.structs = []
        self.struct_typedefs = []
        self.struct_names = set()
        self.struct_typedef_names = set()
        self.parser = parser

        if parser is not None:
            for include in parser.includes:
                self.includes.append((include.path, include.system))

            for struct in parser.structs:
                self.structs.append(struct)
                self.struct_names.add(struct[0])

            for struct_typedef in parser.struct_typedefs:
                self.struct_typedefs.append(struct_typedef)
                self.struct_typedef_names.add(struct_typedef[0])

    def add_mock(self,
                 mocked_function,
                 has_implementation,
                 real_variadic_function):
        self.mocks.append(FunctionMock(mocked_function,
                                       has_implementation,
                                       real_variadic_function,
                                       self))

    def lookup_typedef(self, name):
        if name in self.parser.typedefs:
            return self.parser.typedefs[name]

    def write_to_directory(self, directory):
        os.makedirs(directory, exist_ok=True)

        header_filename = header_file(directory)
        source_filename = source_file(directory)
        linker_filename = linker_file(directory)

        mocks = sorted(self.mocks, key=lambda m: m.func_name)
        struct_asserts = []

        for struct in sorted(self.structs):
            struct_asserts.append(StructAssert(struct,
                                               self.struct_names,
                                               self.struct_typedef_names))

        struct_typedef_asserts = []

        for typedef in sorted(self.struct_typedefs):
            struct_typedef_asserts.append(
                StructTypedefAssert(typedef,
                                    self.struct_names,
                                    self.struct_typedef_names))

        header_code = self.header_template.render(
            nala_version=__version__,
            guard_name=get_guard_name(header_filename),
            includes=generate_includes(self.includes, directory),
            mocks=mocks,
            struct_asserts=struct_asserts,
            struct_typedef_asserts=struct_typedef_asserts)

        includes = [
            ('stddef.h', True),
            ('errno.h', True),
            (header_filename, False)
        ]
        source_code = self.source_template.render(
            nala_version=__version__,
            includes=generate_includes(includes, directory),
            nala_c=read_nala_c(),
            mocks=mocks,
            struct_asserts=struct_asserts,
            struct_typedef_asserts=struct_typedef_asserts)

        with open(header_filename, 'w') as fout:
            fout.write(header_code.strip())
            fout.write('\n')

        with open(source_filename, 'w') as fout:
            fout.write(source_code.strip())
            fout.write('\n')

        with open(linker_filename, 'w') as fout:
            fout.write(' '.join([
                f'-Wl,--wrap={mock.function.name}'
                for mock in mocks
            ]))


def header_file(directory):
    return os.path.join(directory, HEADER_FILE)


def source_file(directory):
    return os.path.join(directory, SOURCE_FILE)


def linker_file(directory):
    return os.path.join(directory, LINKER_FILE)


def does_generated_files_exist(directory):
    if not os.path.exists(header_file(directory)):
        return False

    if not os.path.exists(source_file(directory)):
        return False

    if not os.path.exists(linker_file(directory)):
        return False

    return True
