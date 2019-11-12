from .inspect import collect_mocked_functions
from .generator import GeneratedMock
from .generator import FileGenerator


def generate_mocks(expanded_code):
    """Identify mocked functions and generate the source and header files.

    """

    generator = FileGenerator()

    for function in collect_mocked_functions(expanded_code):
        generator.add_mock(function)

    generator.write_to_directory('.')
