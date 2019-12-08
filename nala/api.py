from .inspect import collect_mocked_functions
from .generator import GeneratedMock
from .generator import FileGenerator


NALA_C_FUNCTIONS = [
    'malloc',
    'free',
    'printf',
    'fprintf',
    'snprintf',
    'fread',
    'memcpy',
    'strcmp',
    'strncmp'
]

NALA_C_CAPTURE_FUNCTIONS = [
    'fseek',
    'ftell',
    'fflush',
    'dup',
    'dup2',
    'fclose',
    'exit',
]


def generate_mocks(expanded_code,
                   output_directory,
                   rename_parameters_file,
                   redefine_syms):
    """Identify mocked functions and generate the source and header files.

    """

    generator = FileGenerator()

    for function in collect_mocked_functions(expanded_code, rename_parameters_file):
        if function.name in NALA_C_FUNCTIONS:
            raise Exception(
                f"'{function.name}()' cannot be mocked as it is used by Nala.")

        if function.name in NALA_C_CAPTURE_FUNCTIONS:
            print(f"warning: '{function.name}()' cannot be mocked if "
                  f"capturing output.")

        generator.add_mock(function, redefine_syms)

    generator.write_to_directory(output_directory, redefine_syms)
