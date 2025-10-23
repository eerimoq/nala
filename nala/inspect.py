"""Module in charge of detecting mock usage in expanded source code."""

import os
import sys
import re
from typing import NamedTuple
from typing import Tuple

from pycparser import c_ast
from pycparser.c_parser import CParser


class IncludeDirective(NamedTuple):

    path: str
    system: bool

    @classmethod
    def from_source_context(cls, source_context):
        for filename in source_context:
            dirname, basename = os.path.split(filename)
            fullname = basename

            while True:
                dirname, basename = os.path.split(dirname)

                if 'include' in basename.lower():
                    return cls(fullname, True)

                if not basename:
                    break

                fullname = os.path.join(basename, fullname)

        return cls(os.path.abspath(source_context[-1]), False)


class MockedFunction(NamedTuple):

    name: str
    declaration: c_ast.FuncDecl


class Token(NamedTuple):

    type: str
    value: str
    span: Tuple[int, int]

    def is_punctuation(self, *symbols):
        return self.type == 'PUNCTUATION' and self.value in symbols

    def is_keyword(self, *keywords):
        return self.type == 'KEYWORD' and self.value in keywords

    @property
    def is_prefix(self):
        return self.type == 'KEYWORD' and self.value in (
            'typedef',
            'extern',
            'static',
            'auto',
            'register',
            '__extension__'
        )


def load_param_names(filename):
    param_names = {}

    with open(filename, 'r') as fin:
        for line in fin:
            line = line.strip()

            if not line:
                continue

            if line.startswith('#'):
                continue

            mo = re.match(r'([^(]+)\(([^)]+)\)', line)

            if not mo:
                raise Exception(f"Invalid parameter name line '{line}'.")

            param_names[mo.group(1)] = [
                param_name.strip()
                for param_name in mo.group(2).split(',')
            ]

    return param_names


def rename_parameters(function_declaration, param_names):
    if function_declaration.type.args is None:
        return function_declaration

    params = []

    for param in function_declaration.type.args.params:
        if isinstance(param, c_ast.Typename):
            param = c_ast.Decl(param.name,
                               [],
                               None,
                               [],
                               [],
                               param.type,
                               None,
                               None)

        params.append(param)

    function_declaration.type.args.params = params

    for i, param in enumerate(function_declaration.type.args.params):
        if isinstance(param, c_ast.EllipsisParam):
            continue

        param_type = param.type

        if (not param.name
            and isinstance(param_type.type, c_ast.IdentifierType)
            and param_type.type.names == ['void']):
            continue

        if param_names is not None:
            param.name = param_names[i]
        elif param.name is not None:
            if len(param.name) > 2 and param.name.startswith('__'):
                param.name = param.name[2:]

        while not isinstance(param_type, c_ast.TypeDecl):
            param_type = param_type.type

        param_type.declname = param.name

    return function_declaration


LINEMARKER = re.compile(r'^# \d+ "((?:\\.|[^\\"])*)"((?: [1234])*)$')

TOKENS = {
    'LINEMARKER': r"^# \d+ .*$",
    'PRAGMA': r'^#pragma.*$',
    'KEYWORD': (
        "\\b(?:auto|break|case|char|const|continue|default|do|double|else|enum|"
        "extern|float"
        "|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct"
        "|switch|typedef|union|unsigned|void|volatile|while|__extension__|"
        "__attribute__|__restrict|__signed__|__signed|_Nullable)\\b"
    ),
    'IDENTIFIER': r"\b[a-zA-Z_](?:[a-zA-Z_0-9])*\b",
    'CHARACTER': r"L?'(?:\\.|[^\\'])+'",
    'STRING': r'L?"(\\"|\\\\|.)*?"',
    'INTEGER': r"(?:0[xX][a-fA-F0-9]+|[0-9]+)[uUlL]*",
    'FLOAT': (
        r"(?:[0-9]+[Ee][+-]?[0-9]+|[0-9]*\.[0-9]+(?:[Ee][+-]?[0-9]+)?|[0-9]+\."
        r"[0-9]*(?:[Ee][+-]?[0-9]+)?)[fFlL]?"
    ),
    'PUNCTUATION': (
        r"\.\.\.|>>=|<<=|\+=|-=|\*=|/=|%=|&=|\^=|\|=|>>|<<|\+\+|--|->|&&|\|\|""|<="
        r"|>=|==|!=|;|\{|\}|,|:|=|\(|\)|\[|\]|\.|&|!|~|-|\+|\*|/|%|<|>|\^|\||\?"
    ),
    'SPACE': r"[ \t\v\n\f]*",
    'IGNORE': r".+?",
}

IGNORED_TOKENS = set(['SPACE', 'IGNORE'])

RE_TOKEN = re.compile(
    '|'.join(f'(?P<{token}>{pattern})' for token, pattern in TOKENS.items()),
    flags=re.MULTILINE)


class PrimitiveType(NamedTuple):

    name: str


class VaList(NamedTuple):

    pass


class VoidType(NamedTuple):

    pass


def is_types(type_, types):
    if isinstance(type_, types):
        return True

    if isinstance(type_, c_ast.TypeDecl):
        return is_types(type_.type, types)

    return False


def is_struct(type_):
    return is_types(type_, c_ast.Struct)


def is_struct_or_union(type_):
    return is_types(type_, (c_ast.Struct, c_ast.Union))


def is_pointer_or_array(type_):
    return is_types(type_, (c_ast.PtrDecl, c_ast.ArrayDecl))


def is_pointer(type_):
    return is_types(type_, c_ast.PtrDecl)


def is_array(type_):
    return is_types(type_, c_ast.ArrayDecl)


def is_pointer_to_pointer(type_):
    if isinstance(type_, c_ast.PtrDecl):
        return is_pointer(type_.type)

    if isinstance(type_, c_ast.TypeDecl):
        return is_pointer_to_pointer(type_.type)

    return False


def is_va_list(type_):
    if isinstance(type_, c_ast.IdentifierType):
        return type_.names[0] == '__builtin_va_list'

    if isinstance(type_, c_ast.TypeDecl):
        return is_va_list(type_.type)

    return False


def is_fixed_array(type_):
    if isinstance(type_, c_ast.ArrayDecl):
        return type_.dim is not None

    if isinstance(type_, c_ast.TypeDecl):
        return is_fixed_array(type_.type)

    return False


def is_primitive_type(type_):
    PRIMITIVE_TYPES = set([
        'char', 'signed', 'unsigned',
        'short', 'int', 'long',
        'float', 'double',
    ])

    if isinstance(type_, c_ast.IdentifierType):
        return set(type_.names) <= PRIMITIVE_TYPES

    return False


def is_primitive_type_pointer_type(type_):
    if isinstance(type_, c_ast.IdentifierType):
        return is_primitive_type(type_)

    if isinstance(type_, c_ast.TypeDecl):
        return is_primitive_type_pointer_type(type_.type)

    return False


def is_primitive_type_pointer(type_):
    if isinstance(type_, c_ast.TypeDecl):
        return is_primitive_type_pointer(type_.type)

    if isinstance(type_, c_ast.PtrDecl):
        return is_primitive_type_pointer_type(type_.type)

    return False


def is_char_pointer(type_):
    if not isinstance(type_, c_ast.PtrDecl):
        return False

    if not isinstance(type_.type, c_ast.TypeDecl):
        return False

    if not isinstance(type_.type.type, c_ast.IdentifierType):
        return False

    return ' '.join(type_.type.type.names) == 'char'


class ForgivingDeclarationParser:

    def __init__(self, source_code, functions, rename_parameters_file=None):
        self.source_code = source_code
        self.functions = functions
        self.token_stream = self.tokenize(source_code)
        self.previous = None
        self.current = None
        self.current_file = None

        self.chunks_to_erase = []
        self.bracket_stack = []
        self.source_context = []
        self.typedefs_code = ['typedef int __builtin_va_list;']
        self.typedefs = {}
        self.structs_code = []
        self.structs = []
        self.struct_typedefs = []
        self.includes = []

        self.cparser = CParser()
        self.param_names = None

        if rename_parameters_file is not None:
            self.param_names = load_param_names(rename_parameters_file)

        self.func_names = []
        self.func_signatures = []
        self.file_ast = None
        self.mocked_functions = []
        self.parse()

        if self.functions:
            for function in sorted(functions):
                print(f"error: Mocked function '{function}' undeclared. Add "
                      "missing include in the test file.",
                      file=sys.stderr)

            raise Exception(
                'Unable to find declarations of all mocked functions. Add missing '
                'include(s) in the test file.')

    @classmethod
    def tokenize(cls, source_code):
        for match in RE_TOKEN.finditer(source_code):
            if match.lastgroup not in IGNORED_TOKENS:
                yield Token(match.lastgroup, match.group().strip(), match.span())

    def parse(self):
        while self.next():
            if self.current.is_keyword('typedef'):
                self.parse_typedef()

            parsed = self.parse_function_declaration_or_struct()

            if parsed is not None:
                self.func_names.append(parsed[0])
                self.func_signatures.append(parsed[1])
                self.functions.remove(parsed[0])

            if not self.functions:
                break

            while self.bracket_stack or not self.current.is_punctuation(';', '}'):
                self.next()

        if self.functions:
            return

        code = '\n'.join(
            self.typedefs_code + self.structs_code + self.func_signatures)
        self.file_ast = self.cparser.parse(code)
        func_offset = len(self.typedefs_code + self.structs_code)
        # PATCH BEGIN
        # The above offset calculation sometimes ends up giving a wrong start index.
        # As a result the first function_declaration handed over to `rename_parameters`
        # is of type 'Struct' which has no argument 'args'!
        # Now we test if the element at index `func_offset` has the correct type, and if
        # not the AST list is searched for the first 'c_ast.FuncDecl' starting from the
        # calculated `func_offset`. The index of the first detected 'c_ast.FuncDecl' is
        # then used as new `func_offset`!
        if not isinstance(self.file_ast.ext[func_offset].type, c_ast.FuncDecl):
            for i in range(func_offset, len(self.file_ast.ext)):
                if isinstance(self.file_ast.ext[i].type, c_ast.FuncDecl):
                    func_offset = i
                    break
        # PATCH END

        for i, func_name in enumerate(self.func_names, func_offset):
            if self.param_names is None:
                func_declaration = self.file_ast.ext[i]
            else:
                func_declaration = rename_parameters(
                    self.file_ast.ext[i],
                    self.param_names.get(func_name))

            self.mocked_functions.append(MockedFunction(
                func_name,
                func_declaration))

        self.load_typedefs()
        self.load_structs()

    def resolve_type(self, type_):
        if isinstance(type_, c_ast.IdentifierType):
            name = ' '.join(type_.names)

            if is_primitive_type(type_) or name == '_Bool':
                return PrimitiveType(name)
            elif name == '__builtin_va_list':
                return VaList()
            elif name == 'void':
                return VoidType()
            else:
                return self.resolve_type(self.lookup_typedef(name).type)
        elif isinstance(type_, (c_ast.Union,
                                c_ast.Struct,
                                c_ast.FuncDecl,
                                c_ast.Enum)):
            return type_
        elif isinstance(type_, c_ast.TypeDecl):
            return self.resolve_type(type_.type)
        elif isinstance(type_, c_ast.ArrayDecl):
            if type_.dim is None:
                return self.resolve_type(type_.type)
            else:
                return self.resolve_type(type_.type)
        elif isinstance(type_, c_ast.PtrDecl):
            return self.resolve_type(type_.type)
        else:
            raise Exception(f'Unknown type {type_}.')

    def expand_type(self, type_):
        if isinstance(type_, c_ast.IdentifierType):
            name = ' '.join(type_.names)

            if is_primitive_type(type_):
                pass
            elif name in ['__builtin_va_list', 'void', '_Bool']:
                pass
            else:
                type_ = self.expand_type(self.lookup_typedef(name).type)
        elif isinstance(type_, (c_ast.Union,
                                c_ast.Struct,
                                c_ast.FuncDecl,
                                c_ast.Enum)):
            pass
        elif isinstance(type_, c_ast.TypeDecl):
            type_.type = self.expand_type(type_.type)
        elif isinstance(type_, (c_ast.PtrDecl, c_ast.ArrayDecl)):
            type_.type = self.expand_type(type_.type)
        else:
            raise Exception(f'Unknown type {type_}.')

        return type_

    def lookup_typedef(self, name):
        if name in self.typedefs:
            return self.typedefs[name]

    def load_struct_member(self, member):
        items = []
        expanded_type = self.expand_type(member.type)
        type_ = self.resolve_type(member.type)

        if is_fixed_array(expanded_type):
            items.append(['assert-array-eq', member.name])
        elif is_pointer_or_array(expanded_type):
            pass
        elif isinstance(type_, (PrimitiveType, c_ast.Enum)):
            if member.bitsize is None:
                items.append(['assert-eq', member.name])
            else:
                items.append(['assert-eq-bit-field', member.name, type_.name])
        elif is_struct(expanded_type):
            if type_.name is None:
                for item in self.load_struct_members(type_):
                    item[1] = f'{member.name}.{item[1]}'
                    items.append(item)
            else:
                items.append(['assert-struct', member.name, type_.name])

        return items

    def load_struct_members(self, struct):
        if not struct.decls:
            return []

        items = []

        for member in struct.decls:
            if member.name is None:
                continue

            items += self.load_struct_member(member)

        return items

    def load_structs(self):
        for item in self.file_ast:
            expanded_type = self.expand_type(item.type)

            if not is_struct(expanded_type):
                continue

            type_ = self.resolve_type(expanded_type)

            if isinstance(item, c_ast.Typedef):
                if type_.decls is None:
                    continue

                items = self.load_struct_members(type_)
                self.struct_typedefs.append((item.name, items))
            else:
                items = self.load_struct_members(type_)
                self.structs.append((type_.name, items))

    def load_typedefs(self):
        for item in self.file_ast:
            if isinstance(item, c_ast.Typedef):
                self.typedefs[item.name] = item

    def next(self):
        self.previous = self.current
        self.current = next(self.token_stream, None)

        if not self.current:
            return None

        if self.current.type == 'PUNCTUATION':
            if self.current.value in '({[':
                self.bracket_stack.append(')}]'['({['.index(self.current.value)])
            elif self.bracket_stack and self.current.value == self.bracket_stack[-1]:
                self.bracket_stack.pop()
        elif self.current.type == 'LINEMARKER':
            filename, flags = LINEMARKER.match(self.current.value).groups()

            if not flags and len(self.source_context) == 0:
                self.current_file = filename

            if self.current_file not in ['<built-in>', '<command-line>']:
                if '1' in flags:
                    self.source_context.append(filename)

                    if len(self.source_context) == 1:
                        self.includes.append(
                            IncludeDirective.from_source_context(
                                self.source_context))
                elif '2' in flags:
                    self.source_context.pop()

            self.mark_for_erase(*self.current.span)
            self.next()
        elif self.current.is_keyword('__attribute__'):
            begin = self.current.span[0]
            stack_depth = len(self.bracket_stack)
            self.next()

            while len(self.bracket_stack) > stack_depth:
                self.next()

            self.mark_for_erase(begin, self.current.span[1])
        elif self.current.is_keyword('__extension__',
                                     '__restrict',
                                     '__signed__',
                                     '__signed',
                                     '_Nullable'):
            self.mark_for_erase(*self.current.span)
            self.next()
        elif self.current.type == 'PRAGMA':
            self.mark_for_erase(*self.current.span)
            self.next()

        return self.current

    def parse_typedef(self):
        begin = self.current.span[0]

        while self.bracket_stack or not self.current.is_punctuation(';'):
            self.next()

        code = self.read_source_code(begin, self.current.span[1])
        self.typedefs_code.append(code)

    def parse_struct(self, begin, _name):
        while self.bracket_stack:
            self.next()

        code = self.read_source_code(begin, self.current.span[1]) + ';'

        if self.is_in_header_file():
            self.structs_code.append(code)

    def is_in_header_file(self):
        return len(self.source_context) > 0

    def parse_function_declaration_or_struct(self):
        while self.current.is_prefix:
            self.next()

        begin = self.current.span[0]
        return_type = []

        if self.current.is_keyword('struct'):
            if self.next() and self.current.type == 'IDENTIFIER':
                struct_name = self.current.value

                if self.next() and self.current.value == '{':
                    self.parse_struct(begin, struct_name)

                    return None

        while (not self.current.is_punctuation('(')
               or self.next()
               and self.current.is_punctuation('*')):
            if not self.bracket_stack and self.current.is_punctuation(';'):
                return None

            return_type.append(self.current.value)
            self.next()

        if not return_type:
            return None

        func_name = return_type.pop()

        if func_name not in self.functions:
            return None

        while (self.bracket_stack
               or self.next()
               and self.current.is_punctuation('(')):
            self.next()

        code = self.read_source_code(begin, self.previous.span[1]) + ';'

        return func_name, code

    def mark_for_erase(self, begin, end):
        self.chunks_to_erase.append((begin, end))

    def read_source_code(self, begin, end):
        if self.chunks_to_erase:
            chunks = []
            offset = begin

            for chunk_begin, chunk_end in self.chunks_to_erase:
                if chunk_end < offset:
                    continue

                chunks.append(self.source_code[offset:chunk_begin])
                offset = chunk_end

            chunks.append(self.source_code[offset:end])
            self.chunks_to_erase = []
            code = ''.join(chunks)
        else:
            code = self.source_code[begin:end]

        return code.strip()
