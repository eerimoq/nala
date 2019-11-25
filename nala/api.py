from .inspect import collect_mocked_functions
from .generator import GeneratedMock
from .generator import FileGenerator


NALA_C_FUNCTIONS = [
    'malloc',
    'free',
    'printf',
    'fprintf',
    'snprintf',
    'fseek',
    'ftell',
    'fflush',
    'dup',
    'dup2',
    'fread',
    'fclose',
    'exit',
    'fgets',
    'memcpy',
    'strcmp',
    'strncmp',
    'strdup',
    'perror'
]


def generate_mocks(expanded_code, output_directory, rename_parameters_file):
    """Identify mocked functions and generate the source and header files.

    """

    generator = FileGenerator()

    for function in collect_mocked_functions(expanded_code, rename_parameters_file):
        if function.name in NALA_C_FUNCTIONS:
            raise Exception(
                f"'{function.name}()' cannot be mocked as it is used by Nala.")

        generator.add_mock(function)

    generator.write_to_directory(output_directory)
