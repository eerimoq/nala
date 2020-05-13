import os
import re
from fnmatch import fnmatch

from .inspect import ForgivingDeclarationParser
from .generator import FileGenerator
from .generator import header_file
from .generator import does_generated_files_exist


NALA_C_FUNCTIONS = [
    'snprintf',
    'memcpy',
    'strcmp',
    'strncmp'
]

MOCKED_FUNC_REGEX = re.compile(
    r'(_mock|_mock_once|_mock_ignore_in|_mock_ignore_in_once|_mock_none'
    r'|_mock_implementation)\s*\(')

REAL_VARIADIC_FUNCTIONS_REGEX = re.compile(
    r'// NALA_REAL_VARIADIC_FUNCTION_BEGIN (.*?)\n'
    r'(.*?)\n'
    r'// NALA_REAL_VARIADIC_FUNCTION_END',
    re.MULTILINE | re.DOTALL)


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


def has_implementation(function_name, implementation, no_implementation):
    for pattern in implementation:
        if fnmatch(function_name, pattern):
            return True

    for pattern in no_implementation:
        if fnmatch(function_name, pattern):
            return False

    return None


def load_real_variadic_functions(filename):
    with open(filename, 'r') as fin:
        contents = fin.read()

    return {
        mo[0]: mo[1]
        for mo in REAL_VARIADIC_FUNCTIONS_REGEX.findall(contents)
    }


def generate_mocks(expanded_code,
                   output_directory,
                   rename_parameters_file,
                   real_variadic_functions_file,
                   cache,
                   implementation,
                   no_implementation):
    """Identify mocked functions and generate the source, header and
    linker files.

    """

    functions = find_mocked_functions(expanded_code)

    if cache and does_generated_files_exist(output_directory):
        cached_mocked_functions = find_cached_mocked_functions(
            header_file(output_directory))
        generate = (functions != cached_mocked_functions)
    else:
        generate = True

    if not generate:
        return

    parser = ForgivingDeclarationParser(expanded_code,
                                        functions,
                                        rename_parameters_file)

    if real_variadic_functions_file:
        real_variadic_functions = load_real_variadic_functions(
            real_variadic_functions_file)
    else:
        real_variadic_functions = {}

    generator = FileGenerator(parser)

    for function in parser.mocked_functions:
        if function.name in NALA_C_FUNCTIONS:
            raise Exception(
                f"'{function.name}()' cannot be mocked as it is used by Nala.")

        generator.add_mock(function,
                           has_implementation(function.name,
                                              implementation,
                                              no_implementation),
                           real_variadic_functions.get(function.name, ''))

    generator.write_to_directory(output_directory)
