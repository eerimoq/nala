"""Module in charge of detecting mock usage in expanded source code."""

import os
import sys
import re
from typing import NamedTuple

from pycparser import c_ast as node
from pycparser.c_parser import CParser

from .cparser import fast_parse


def collect_mocked_functions(expanded_source_code,
                             functions,
                             rename_parameters_file):
    """Yield all the mocked functions used in the expanded source code."""

    yield from ForgivingDeclarationParser(expanded_source_code,
                                          functions,
                                          rename_parameters_file)

    if functions:
        for function in sorted(functions):
            print(f"error: Mocked function '{function}' undeclared. Missing include?",
                  file=sys.stderr)

        raise Exception(
            "Unable to find declarations of all mocked functions. Add missing "
            "includes to the test file.")


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

    def __init__(self, source_code, functions, rename_parameters_file=None):
        self.source_code = source_code
        self.functions = functions
        self.cparser = CParser()
        self.param_names = {}

        if rename_parameters_file is not None:
            self.param_names = load_param_names(rename_parameters_file)

        self.mocked_functions = []
        self.parse()

    def parse(self):
        result = fast_parse(self.source_code, self.functions)

        if self.functions:
            return

        result.typedefs.insert(0, "typedef int __builtin_va_list;")
        code = '\n'.join(result.typedefs + result.func_signatures)
        code = code.replace('__extension__', '')
        code = code.replace('__restrict', '')
        code = code.replace('__signed__', 'signed')
        file_ast = self.cparser.parse(code)
        items = zip(result.func_names, result.func_source_contexts)

        for i, (func_name, source_context) in enumerate(items, len(result.typedefs)):
            param_names = self.param_names.get(func_name)

            if param_names:
                func_declaration = rename_parameters(file_ast.ext[i],
                                                     param_names)
            else:
                func_declaration = file_ast.ext[i]

            self.mocked_functions.append(MockedFunction(
                func_name,
                func_declaration,
                IncludeDirective.from_source_context(source_context),
                file_ast))

    def __iter__(self):
        for mocked_function in self.mocked_functions:
            yield mocked_function
