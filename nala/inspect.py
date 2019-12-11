"""Module in charge of detecting mock usage in expanded source code."""

import os
import sys
import re
from typing import NamedTuple
from typing import Tuple

from pycparser import c_ast as node
from pycparser.c_parser import CParser
from pycparser.plyparser import ParseError


def collect_mocked_functions(expanded_source_code,
                             functions,
                             rename_parameters_file):
    """Yield all the mocked functions used in the expanded source code."""

    yield from ForgivingDeclarationParser(expanded_source_code,
                                          functions,
                                          rename_parameters_file)

    if functions:
        for function in functions:
            print(f"error: '{function}' undeclared", file=sys.stderr)

        sys.exit(1)


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
    include: IncludeDirective
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


class ForgivingDeclarationParser:
    linemarker = re.compile(r'^# \d+ "((?:\\.|[^\\"])*)"((?: [1234])*)$')

    tokens = {
        "LINEMARKER": r"^#.*$",
        "KEYWORD": (
            "\\b(?:auto|break|case|char|const|continue|default|do|double|else|enum|extern|float"
            "|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch"
            "|typedef|union|unsigned|void|volatile|while|__extension__|__attribute__|__restrict)\\b"
        ),
        "IDENTIFIER": r"\b[a-zA-Z_](?:[a-zA-Z_0-9])*\b",
        "CHARACTER": r"L?'(?:\\.|[^\\'])+'",
        "STRING": r'L?"(?:\\.|[^\\"])*"',
        "INTEGER": r"(?:0[xX][a-fA-F0-9]+|[0-9]+)[uUlL]*",
        "FLOAT": (
            r"(?:[0-9]+[Ee][+-]?[0-9]+|[0-9]*\.[0-9]+(?:[Ee][+-]?[0-9]+)?|[0-9]+\.[0-9]*(?:[Ee][+-]?[0-9]+)?)[fFlL]?"
        ),
        "PUNCTUATION": (
            r"\.\.\.|>>=|<<=|\+=|-=|\*=|/=|%=|&=|\^=|\|=|>>|<<|\+\+|--|->|&&|\|\||<=|>=|"
            r"==|!=|;|\{|\}|,|:|=|\(|\)|\[|\]|\.|&|!|~|-|\+|\*|/|%|<|>|\^|\||\?"
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

    @classmethod
    def tokenize(cls, source_code):
        for match in cls.regex.finditer(source_code):
            if match.lastgroup not in cls.ignored_tokens:
                yield Token(match.lastgroup, match.group().strip(), match.span())

    def parse(self):
        while self.next():
            if self.current.is_keyword("typedef"):
                self.parse_typedef()

            parsed = self.parse_function_declaration()

            if parsed is not None:
                self.func_names.append(parsed[0])
                self.func_signatures.append(parsed[1])
                self.func_source_contexts.append(self.source_context[:])
                self.functions.remove(parsed[0])

            if not self.functions:
                break

            while (self.current and not (self.current.is_punctuation(";", "}")
                                         and not self.bracket_stack)):
                self.next()

        code = '\n'.join(self.typedefs + self.func_signatures)

        try:
            self.file_ast = self.cparser.parse(code)
        except ParseError:
            pass

        items = zip(self.func_names, self.func_source_contexts)

        for i, (func_name, source_context) in enumerate(items, len(self.typedefs)):
            param_names = self.param_names.get(func_name)

            if param_names:
                func_declaration = rename_parameters(self.file_ast.ext[i],
                                                     param_names)
            else:
                func_declaration = self.file_ast.ext[i]

            self.mocked_functions.append(MockedFunction(
                func_name,
                func_declaration,
                IncludeDirective.from_source_context(source_context),
                self.file_ast))

    def __iter__(self):
        for mocked_function in self.mocked_functions:
            yield mocked_function

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

            if "1" in flags:
                self.source_context.append(filename)
            elif "2" in flags:
                self.source_context.pop()

            try:
                self.source_context[-1] = filename
            except IndexError:
                self.source_context.append(filename)

            self.erase_code_section(*self.current.span)
            self.next()
        elif self.current.is_keyword("__attribute__"):
            begin = self.current.span[0]

            stack_depth = len(self.bracket_stack)
            self.next()

            while len(self.bracket_stack) > stack_depth:
                self.next()

            self.erase_code_section(begin, self.current.span[1])
        elif self.current.is_keyword("__extension__", "__restrict"):
            self.erase_code_section(*self.current.span)
            self.next()

        return self.current

    def parse_typedef(self):
        start_index = self.current.span[0]

        while (self.current
               and not (self.current.is_punctuation(";") and not self.bracket_stack)):
            self.next()

        code = self.read_source_code(start_index, self.current.span[1])
        # typedef __signed__ char __s8;
        self.typedefs.append(code.replace('__signed__', 'signed'))

    def parse_function_declaration(self):
        if self.bracket_stack:
            return None

        while self.current and self.current.is_prefix:
            self.next()

        start_index = self.current.span[0]
        return_type = []

        while (self.current
               and not self.current.is_punctuation("(")
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

        while (self.current
               and self.bracket_stack
               or self.next()
               and self.current.is_punctuation("(")):
            self.next()

        code = self.read_source_code(start_index, self.previous.span[1]) + ";"
            
        return func_name, code

    def erase_code_section(self, begin, end):
        self.chunks_to_erase.append((begin, end))

    def read_source_code(self, begin, end):
        self.perform_erase()

        return self.source_code[begin:end]
        
    def perform_erase(self):
        chunks = []
        offset = 0

        for begin, end in self.chunks_to_erase:
            chunks.append(self.source_code[offset:begin])
            chunks.append(' ' * (end - begin))
            offset = end

        chunks.append(self.source_code[offset:])

        self.source_code = ''.join(chunks)
        self.chunks_to_erase = []
