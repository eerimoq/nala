"""Module in charge of generating mocks."""

import os
import re
from copy import deepcopy

from jinja2 import Environment
from jinja2 import PackageLoader
from pycparser import c_ast as node
from pycparser.c_generator import CGenerator

from . import __version__


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

HEADER_FILE = "nala_mocks.h"
SOURCE_FILE = "nala_mocks.c"
LINKER_FILE = "nala_mocks.ldflags"


def is_ellipsis(param):
    return isinstance(param, node.EllipsisParam)


def is_va_list(param):
    try:
        return param.type.type.names[0] == 'va_list'
    except AttributeError:
        return False


def decl(name, type):
    return node.Decl(name, [], [], [], type, None, None)


def function_ptr_decl(name, return_type, parameters):
    return decl(
        name, node.PtrDecl([], node.FuncDecl(node.ParamList(parameters), return_type))
    )


def bool_param(name):
    return decl(name, node.TypeDecl(name, [], node.IdentifierType(["bool"])))


def set_member(name):
    return decl(name,
                node.TypeDecl(name,
                              [],
                              node.IdentifierType(["struct nala_set_param"])))


def in_assert_member(param):
    name = f'{param.name}_in_assert'

    return function_ptr_decl(
        name,
        void_type(name),
        [
            param,
            decl('nala_buf_p',
                 node.PtrDecl([],
                              node.TypeDecl('nala_buf_p',
                                            ['const'],
                                            node.IdentifierType(["void"])))),
            decl('nala_size',
                 node.TypeDecl('nala_size', [], node.IdentifierType(["size_t"])))
        ])


def out_copy_member(param):
    name = f'{param.name}_out_copy'

    return function_ptr_decl(
        name,
        void_type(name),
        [
            param,
            decl('nala_buf_p',
                 node.PtrDecl([],
                              node.TypeDecl('nala_buf_p',
                                            ['const'],
                                            node.IdentifierType(["void"])))),
            decl('nala_size',
                 node.TypeDecl('nala_size', [], node.IdentifierType(["size_t"])))
        ])


def va_list_param(name):
    return decl(None,
                node.TypeDecl(name, [], node.IdentifierType(["va_list"])))


def is_variadic_func(params):
    if params:
        return is_ellipsis(params[-1])
    else:
        return False


def void_type(name):
    return node.TypeDecl(name, [], node.IdentifierType(['void']))


def int_type(name):
    return node.TypeDecl(name, [], node.IdentifierType(['int']))


def rename_return_type(return_type, name):
    return_type = deepcopy(return_type)
    type_decl = return_type

    while not isinstance(type_decl, node.TypeDecl):
        type_decl = type_decl.type

    type_decl.declname = name

    return return_type


def create_implementation_params(params):
    return [
        va_list_param("__nala_va_list")
        if is_ellipsis(param)
        else param
        for param in params
    ]


def get_guard_name(filename):
    slug = re.sub(r"[^a-zA-Z0-9]", "_", os.path.normpath(os.path.relpath(filename)))

    return re.sub(r"_+", "_", slug).upper().strip("_")


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


class StructAssertIn:

    def __init__(self, struct):
        self.name = struct[0]
        self.assert_in_members = []

        for member in struct[1]:
            if member[0] == 'assert-eq':
                self.assert_in_members.append(member[1])


def find_real_variadic_code(function):
    if function.name == 'open':
        return REAL_VARIADIC_OPEN
    else:
        return ''


class FunctionMock:

    DECL_MARKER = "// NALA_DECLARATION"
    IMPL_MARKER = "// NALA_IMPLEMENTATION"

    def __init__(self, function, has_implementation, real_variadic_function):
        self.function = function
        self.func_name = function.name
        self.real_variadic_function = real_variadic_function

        self.wrapped_func = f"__wrap_{self.func_name}"
        self.real_func = f"__real_{self.func_name}"

        self.state_name = f"nala_mock_{self.func_name}"

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


        self.params_struct = [
            decl(param.name, node.PtrDecl([], param.type.type))
            if isinstance(param.type, node.ArrayDecl)
            else param
            for param in self.func_params
            if not is_ellipsis(param) and param.name
        ]
        self.forward_args = ", ".join(param.name for param in self.params_struct)

        if self.is_variadic_func:
            self.params_struct.append(decl(
                None,
                node.PtrDecl([],
                             node.TypeDecl("vafmt_p",
                                           ['const'],
                                           node.IdentifierType(["char"])))))
            self.forward_args += ', nala_vl'

        # -Wpedantic warns on empty structs.
        if not self.params_struct:
            self.params_struct = [
                decl("dummy",
                     node.TypeDecl("dummy", [], node.IdentifierType(["int"])))
            ]

        return_type = self.func_decl.type
        self.return_value = (
            None
            if isinstance(return_type, node.TypeDecl)
            and isinstance(return_type.type, node.IdentifierType)
            and return_type.type.names[0] == "void"
            else "return_value")

        if self.is_variadic_func:
            self.va_list_start_arg_name = self.func_params[-2].name
        else:
            self.va_list_start_arg_name = None

        self.return_value_decl = decl(
            self.return_value,
            rename_return_type(return_type, self.return_value))
        mock_params = self.create_mock_params()
        self.implementation_decl = function_ptr_decl(
            "implementation",
            rename_return_type(return_type, "implementation"),
            create_implementation_params(self.func_params))
        self.mock_func = self.void_function_decl(f'{self.func_name}_mock',
                                                 mock_params)
        self.mock_once_func = self.int_function_decl(
            f'{self.func_name}_mock_once',
            mock_params)
        self.set_errno_func = self.void_function_decl(
            f'{self.func_name}_mock_set_errno',
            [decl(
                "errno_value",
                node.TypeDecl("errno_value", [], node.IdentifierType(["int"])),
            )])
        self.callback_decl = function_ptr_decl(
            "callback",
            void_type("callback"),
            create_implementation_params(self.func_params))
        self.variadic_func_real_wrapper_decl = node.FuncDecl(
            node.ParamList(create_implementation_params(self.func_params)),
            node.TypeDecl(
                f'{self.func_name}_mock_va_arg_real',
                [],
                return_type))
        self.default_variadic_func_real_wrapper_decl = node.FuncDecl(
            node.ParamList(create_implementation_params(self.func_params)),
            node.TypeDecl(
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
            elif is_va_list(param):
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

            self.instance_members.append(set_member(f'{param.name}_in'))
            self.instance_members.append(in_assert_member(param))
            self.instance_members.append(set_member(f'{param.name}_out'))
            self.instance_members.append(out_copy_member(param))
            self.set_params.append((param, self.find_check_function(param)))

    def assign_names_to_unnamed_params(self, params):
        for i, param in enumerate(params):
            if not isinstance(param, node.Typename):
                continue

            if self.is_void(param):
                continue

            name = f'nala_{i}'
            param.name = name

            while not isinstance(param, node.TypeDecl):
                param = param.type

            param.declname = name

    def find_check_function(self, param):
        if self.is_char_pointer(param):
            return 'nala_mock_assert_string'
        elif isinstance(param.type.type, node.TypeDecl):
            if isinstance(param.type.type.type, node.Struct):
                return f'nala_mock_assert_in_struct_{param.type.type.type.name}'

        return 'nala_mock_assert_memory'

    def void_function_decl(self, name, parameters):
        return node.FuncDecl(node.ParamList(parameters),
                             void_type(name))

    def int_function_decl(self, name, parameters):
        return node.FuncDecl(node.ParamList(parameters),
                             int_type(name))

    def rename_function(self, name):
        return decl(
            name,
            node.FuncDecl(self.func_decl.args,
                          rename_return_type(self.func_decl.type, name)))

    def is_char_pointer(self, param):
        if is_ellipsis(param):
            return False
        elif isinstance(param.type, node.PtrDecl):
            if isinstance(param.type.type, (node.FuncDecl, node.PtrDecl)):
                return False
            elif isinstance(param.type.type.type, node.Struct):
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
        if isinstance(param.type, (node.PtrDecl, node.ArrayDecl)):
            return True

        try:
            name = param.type.type.names[0]

            return self.is_pointer(self.lookup_typedef(name))
        except AttributeError:
            pass

        return False

    def is_enum(self, param):
        return isinstance(param.type.type, node.Enum)

    def is_struct(self, param):
        try:
            if isinstance(param.type.type, node.Struct):
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
            if isinstance(param.type.type, node.Union):
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
        for item in self.function.file_ast:
            if isinstance(item, node.Typedef):
                if item.name == name:
                    return item

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
            elif is_va_list(param):
                continue
            elif self.is_char_pointer_or_non_pointer(param):
                once_params.append(param)
            elif is_ellipsis(param):
                variable_arguments_params.append(decl(
                    None,
                    node.PtrDecl([],
                                 node.TypeDecl("vafmt_p",
                                               ['const'],
                                               node.IdentifierType(["char"])))))
                variable_arguments_params.append(param)

        if not self.is_void(self.return_value_decl):
            once_params.append(self.return_value_decl)

        once_params += variable_arguments_params

        return once_params


class FileGenerator:

    def __init__(self):
        self.code_generator = CGenerator()

        self.jinja_env = Environment(
            loader=PackageLoader("nala", "templates"),
            trim_blocks=True,
            lstrip_blocks=True,
        )
        self.jinja_env.filters["render"] = self.code_generator.visit

        self.header_template = self.jinja_env.get_template(f"{HEADER_FILE}.jinja2")
        self.source_template = self.jinja_env.get_template(f"{SOURCE_FILE}.jinja2")

        self.mocks = []
        self.struct_assert_ins = []
        self.includes = []

    def add_mock(self,
                 mocked_function,
                 has_implementation,
                 real_variadic_function):
        self.mocks.append(FunctionMock(mocked_function,
                                       has_implementation,
                                       real_variadic_function))

    def add_include(self, include):
        self.includes.append((include.path, include.system))

    def add_struct(self, struct):
        self.struct_assert_ins.append(StructAssertIn(struct))

    def write_to_directory(self, directory):
        os.makedirs(directory, exist_ok=True)

        header_filename = os.path.join(directory, HEADER_FILE)
        source_filename = os.path.join(directory, SOURCE_FILE)
        linker_filename = os.path.join(directory, LINKER_FILE)

        mocks = list(sorted(self.mocks, key=lambda m: m.func_name))
        struct_assert_ins = list(sorted(self.struct_assert_ins,
                                        key=lambda a: a.name))

        header_code = self.header_template.render(
            nala_version=__version__,
            guard_name=get_guard_name(header_filename),
            includes=generate_includes(self.includes, directory),
            mocks=mocks,
            struct_assert_ins=struct_assert_ins)

        includes = [
            ("stddef.h", True),
            ("errno.h", True),
            (header_filename, False)
        ]
        source_code = self.source_template.render(
            nala_version=__version__,
            includes=generate_includes(includes, directory),
            nala_c=read_nala_c(),
            mocks=mocks,
            struct_assert_ins=struct_assert_ins)

        with open(header_filename, "w") as fout:
            fout.write(header_code.strip())
            fout.write('\n')

        with open(source_filename, "w") as fout:
            fout.write(source_code.strip())
            fout.write('\n')

        with open(linker_filename, "w") as fout:
            fout.write(' '.join([
                f'-Wl,--wrap={mock.function.name}'
                for mock in mocks
            ]))
