"""High-level public interface."""


__all__ = ["generate_mocks", "collect_linker_flags"]


from .inspect import collect_mocked_functions
from .generator import GeneratedMock, FileGenerator


def generate_mocks(expanded_code, directory):
    """Identify mocked functions and generate the source and header files."""
    generator = FileGenerator()

    for function in collect_mocked_functions(expanded_code):
        generator.add_mock(function)

    generator.write_to_directory(directory)


def collect_linker_flags(directory):
    """Return the flags needed for wrapping the declared mocked functions."""
    try:
        return GeneratedMock.extract_flags(FileGenerator.read_declarations(directory))
    except FileNotFoundError:
        return []
