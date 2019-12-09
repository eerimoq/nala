import os
import re

from .inspect import collect_mocked_functions
from .generator import GeneratedMock
from .generator import FileGenerator
from .generator import HEADER_FILE


NALA_C_FUNCTIONS = [
    'snprintf',
    'memcpy',
    'strcmp',
    'strncmp'
]

GETTER_REGEX = re.compile(
    r"(void )?(\w+?)(_mock|_mock_once|_mock_ignore_in|_mock_ignore_in_once|_mock_none)\s*\(")


def find_mocked_functions(expanded_source_code, nala_mocks_h):
    functions = set()
    changed = True

    for match in GETTER_REGEX.finditer(expanded_source_code):
        void = match.group(1)
        function_name = match.group(2)

        if void is None:
            functions.add(function_name)

    if os.path.exists(nala_mocks_h):
        nala_mocks_functions = set()

        with open(nala_mocks_h, 'r') as fin:
            for line in fin:
                if line.startswith('//'):
                    nala_mocks_functions.add(line.split()[-1])

        changed = (functions != nala_mocks_functions)

    return (functions, changed)


def generate_mocks(expanded_code,
                   output_directory,
                   rename_parameters_file,
                   cache):
    """Identify mocked functions and generate the source and header files.

    """

    nala_mocks_h = os.path.join(output_directory, HEADER_FILE)
    functions, changed = find_mocked_functions(expanded_code, nala_mocks_h)

    if not changed and cache:
        return

    generator = FileGenerator()

    for function in collect_mocked_functions(expanded_code,
                                             functions,
                                             rename_parameters_file):
        if function.name in NALA_C_FUNCTIONS:
            raise Exception(
                f"'{function.name}()' cannot be mocked as it is used by Nala.")

        generator.add_mock(function)

    generator.write_to_directory(output_directory)
