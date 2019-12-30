import os
import re

from .inspect import ForgivingDeclarationParser
from .generator import FileGenerator
from .generator import HEADER_FILE


NALA_C_FUNCTIONS = [
    'snprintf',
    'memcpy',
    'strcmp',
    'strncmp'
]

MOCKED_FUNC_REGEX = re.compile(
    r"(_mock|_mock_once|_mock_ignore_in|_mock_ignore_in_once|_mock_none"
    r"|_mock_implementation)\s*\(")


def find_mocked_function_name(expanded_source_code, index):
    name = ''

    while True:
        index -= 1
        char = expanded_source_code[index]

        if char in ' \t\n\r':
            break

        name += char

    if expanded_source_code[index - 4:index] == 'void':
        return None

    return name[::-1]


def find_mocked_functions(expanded_source_code):
    functions = set()

    for match in MOCKED_FUNC_REGEX.finditer(expanded_source_code):
        function_name = find_mocked_function_name(expanded_source_code,
                                                  match.start())

        if function_name is not None:
            functions.add(function_name)

    return functions


def find_cached_mocked_functions(nala_mocks_h):
    functions = set()

    with open(nala_mocks_h, 'r') as fin:
        for line in fin:
            if line.startswith('// NALA_DECLARATION'):
                functions.add(line.split()[-1])

    return functions


def generate_mocks(expanded_code,
                   output_directory,
                   rename_parameters_file,
                   cache):
    """Identify mocked functions and generate the source and header files.

    """

    functions = find_mocked_functions(expanded_code)
    nala_mocks_h = os.path.join(output_directory, HEADER_FILE)

    if cache and os.path.exists(nala_mocks_h):
        cached_mocked_functions = find_cached_mocked_functions(nala_mocks_h)
        generate = (functions != cached_mocked_functions)
    else:
        generate = True

    generator = FileGenerator()

    if generate:
        parser = ForgivingDeclarationParser(expanded_code,
                                            functions,
                                            rename_parameters_file)

        for struct in parser.structs:
            generator.add_struct(struct)

        for include in parser.includes:
            generator.add_include(include)

        for function in parser.mocked_functions:
            if function.name in NALA_C_FUNCTIONS:
                raise Exception(
                    f"'{function.name}()' cannot be mocked as it is used by Nala.")

            generator.add_mock(function)

        generator.write_to_directory(output_directory)
    elif not functions:
        generator.write_to_directory(output_directory)
