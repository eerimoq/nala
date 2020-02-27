"""Module in charge of detecting mock usage in expanded source code."""

import os
import sys
import re
from typing import NamedTuple
from typing import Tuple

from pycparser import c_ast as node
from pycparser.c_parser import CParser


class IncludeDirective(NamedTuple):
    path: str
    system: bool

    @classmethod
    def from_source_context(cls, source_context):
        if not source_context:
            return None

        for filename in source_context:
            if not os.path.isabs(filename):
                continue

            dirname, basename = os.path.split(filename)
            fullname = basename

            while True:
                dirname, basename = os.path.split(dirname)

                if "include" in basename.lower():
                    return cls(fullname, True)

                if not basename:
                    break

                fullname = os.path.join(basename, fullname)

        return cls(os.path.abspath(source_context[-1]), False)

    def __str__(self):
        return "#include " + (f"<{self.path}>" if self.system else f'"{self.path}"')


class MockedFunction(NamedTuple):
    name: str
    declaration: node.FuncDecl
    file_ast: node.FileAST


class Token(NamedTuple):
    type: str
    value: str
    span: Tuple[int, int]

    def is_punctuation(self, *symbols):
        return self.type == "PUNCTUATION" and self.value in symbols

    def is_keyword(self, *keywords):
        return self.type == "KEYWORD" and self.value in keywords

    @property
    def is_prefix(self):
        return self.type == "KEYWORD" and self.value in (
            "typedef",
            "extern",
            "static",
            "auto",
            "register",
            "__extension__"
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
    for i, param in enumerate(function_declaration.type.args.params):
        param_type = param.type

        if (not param.name
            and isinstance(param_type.type, node.IdentifierType)
            and param_type.type.names == ["void"]):
            continue

        param.name = param_names[i]

        while not isinstance(param_type, node.TypeDecl):
            param_type = param_type.type

        param_type.declname = param.name

    return function_declaration


PRIMITIVE_TYPES = [
    'char',
    'signed char',
    'unsigned char',
    'short',
    'short int',
    'signed short',
    'signed short int',
    'unsigned short',
    'unsigned short int',
    'int',
    'signed',
    'signed int',
    'unsigned',
    'unsigned int',
    'long',
    'long int',
    'signed long',
    'signed long int',
    'unsigned long',
    'unsigned long int',
    'long long',
    'long long int',
    'signed long long',
    'signed long long int',
    'unsigned long long',
    'unsigned long long int',
    'float',
    'double',
    'long double'
]


class ForgivingDeclarationParser:
    linemarker = re.compile(r'^# \d+ "((?:\\.|[^\\"])*)"((?: [1234])*)$')

    tokens = {
        "LINEMARKER": r"^#.*$",
        "KEYWORD": (
            "\\b(?:auto|break|case|char|const|continue|default|do|double|else|enum|"
            "extern|float"
            "|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct"
            "|switch|typedef|union|unsigned|void|volatile|while|__extension__|"
            "__attribute__|__restrict|__signed__)\\b"
        ),
        "IDENTIFIER": r"\b[a-zA-Z_](?:[a-zA-Z_0-9])*\b",
        "CHARACTER": r"L?'(?:\\.|[^\\'])+'",
        "STRING": r'L?"(\\"|\\\\|.)*?"',
        "INTEGER": r"(?:0[xX][a-fA-F0-9]+|[0-9]+)[uUlL]*",
        "FLOAT": (
            r"(?:[0-9]+[Ee][+-]?[0-9]+|[0-9]*\.[0-9]+(?:[Ee][+-]?[0-9]+)?|[0-9]+\."
            r"[0-9]*(?:[Ee][+-]?[0-9]+)?)[fFlL]?"
        ),
        "PUNCTUATION": (
            r"\.\.\.|>>=|<<=|\+=|-=|\*=|/=|%=|&=|\^=|\|=|>>|<<|\+\+|--|->|&&|\|\|""|<="
            r"|>=|==|!=|;|\{|\}|,|:|=|\(|\)|\[|\]|\.|&|!|~|-|\+|\*|/|%|<|>|\^|\||\?"
        ),
        "SPACE": r"[ \t\v\n\f]*",
        "IGNORE": r".+?",
    }

    ignored_tokens = "SPACE", "IGNORE"

    regex = re.compile(
        "|".join(f"(?P<{token}>{pattern})" for token, pattern in tokens.items()),
        flags=re.MULTILINE)

    def __init__(self, source_code, functions, rename_parameters_file=None):
        self.source_code = source_code
        self.functions = functions
        self.token_stream = self.tokenize(source_code)
        self.previous = None
        self.current = None

        self.chunks_to_erase = []
        self.bracket_stack = []
        self.source_context = []
        self.typedefs = ["typedef int __builtin_va_list;"]
        self.structs_code = []
        self.structs = []
        self.includes = []
        self.filename = None

        self.cparser = CParser()
        self.param_names = {}

        if rename_parameters_file is not None:
            self.param_names = load_param_names(rename_parameters_file)

        self.func_names = []
        self.func_signatures = []
        self.func_source_contexts = []
        self.file_ast = None
        self.mocked_functions = []
        self.parse()

        if self.functions:
            for function in sorted(functions):
                print(f"error: Mocked function '{function}' undeclared. Missing include?",
                      file=sys.stderr)

            raise Exception(
                "Unable to find declarations of all mocked functions. Add missing "
                "includes to the test file.")

    @classmethod
    def tokenize(cls, source_code):
        for match in cls.regex.finditer(source_code):
            if match.lastgroup not in cls.ignored_tokens:
                yield Token(match.lastgroup, match.group().strip(), match.span())

    def parse(self):
        while self.next():
            if self.current.is_keyword("typedef"):
                self.parse_typedef()

            parsed = self.parse_function_declaration_or_struct()

            if parsed is not None:
                self.func_names.append(parsed[0])
                self.func_signatures.append(parsed[1])
                self.func_source_contexts.append(self.source_context[:])
                self.functions.remove(parsed[0])

            if not self.functions:
                break

            while self.bracket_stack or not self.current.is_punctuation(";", "}"):
                self.next()

        if self.functions:
            return

        code = '\n'.join(self.typedefs + self.structs_code + self.func_signatures)
        self.file_ast = self.cparser.parse(code)
        items = zip(self.func_names, self.func_source_contexts)
        func_offset = len(self.typedefs + self.structs_code)

        for i, (func_name, _) in enumerate(items, func_offset):
            param_names = self.param_names.get(func_name)

            if param_names:
                func_declaration = rename_parameters(self.file_ast.ext[i],
                                                     param_names)
            else:
                func_declaration = self.file_ast.ext[i]

            self.mocked_functions.append(MockedFunction(
                func_name,
                func_declaration,
                self.file_ast))

        self.load_structs()

    def load_struct_member(self, member):
        item = None

        if isinstance(member.type, node.TypeDecl):
            if isinstance(member.type.type, node.IdentifierType):
                if ' '.join(member.type.type.names) in PRIMITIVE_TYPES:
                    item = ('assert-eq', member.name)

        return item

    def load_struct_members(self, struct):
        items = []

        for member in struct.decls:
            item = self.load_struct_member(member)

            if item is None:
                return []

            items.append(item)

        return items

    def load_structs(self):
        for item in self.file_ast:
            if isinstance(item, node.Typedef):
                if isinstance(item.type, node.TypeDecl):
                    if isinstance(item.type.type, node.Struct):
                        pass
            elif isinstance(item, node.Decl):
                if isinstance(item.type, node.Struct):
                    items = self.load_struct_members(item.type)
                    self.structs.append((item.type.name, items))

    def next(self):
        self.previous = self.current
        self.current = next(self.token_stream, None)

        if not self.current:
            return None

        if self.current.type == "PUNCTUATION":
            if self.current.value in "({[":
                self.bracket_stack.append(")}]"["({[".index(self.current.value)])
            elif self.bracket_stack and self.current.value == self.bracket_stack[-1]:
                self.bracket_stack.pop()
        elif self.current.type == "LINEMARKER":
            filename, flags = self.linemarker.match(self.current.value).groups()

            if self.filename is None:
                self.filename = filename

            if "1" in flags:
                self.source_context.append(filename)

                if len(self.source_context) == 2:
                    if self.source_context[0] == self.filename:
                        self.includes.append(
                            IncludeDirective.from_source_context(self.source_context))
            elif "2" in flags:
                self.source_context.pop()

            try:
                self.source_context[-1] = filename
            except IndexError:
                self.source_context.append(filename)

            self.mark_for_erase(*self.current.span)
            self.next()
        elif self.current.is_keyword("__attribute__"):
            begin = self.current.span[0]
            stack_depth = len(self.bracket_stack)
            self.next()

            while len(self.bracket_stack) > stack_depth:
                self.next()

            self.mark_for_erase(begin, self.current.span[1])
        elif self.current.is_keyword("__extension__", "__restrict", "__signed__"):
            self.mark_for_erase(*self.current.span)
            self.next()

        return self.current

    def parse_typedef(self):
        begin = self.current.span[0]

        while self.bracket_stack or not self.current.is_punctuation(";"):
            self.next()

        code = self.read_source_code(begin, self.current.span[1])
        self.typedefs.append(code)

    def parse_struct(self, begin, _name):
        while self.bracket_stack:
            self.next()

        code = self.read_source_code(begin, self.current.span[1]) + ";"
        self.structs_code.append(code)

    def parse_function_declaration_or_struct(self):
        while self.current.is_prefix:
            self.next()

        begin = self.current.span[0]
        return_type = []

        if self.current.is_keyword("struct"):
            if self.next() and self.current.type == 'IDENTIFIER':
                struct_name = self.current.value

                if self.next() and self.current.value == '{':
                    self.parse_struct(begin, struct_name)

                    return None

        while (not self.current.is_punctuation("(")
               or self.next()
               and self.current.is_punctuation("*")):
            if not self.bracket_stack and self.current.is_punctuation(";"):
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
               and self.current.is_punctuation("(")):
            self.next()

        code = self.read_source_code(begin, self.previous.span[1]) + ";"

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
