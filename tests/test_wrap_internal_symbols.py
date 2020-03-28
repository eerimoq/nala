import unittest
from nala.wrap_internal_symbols import wrap_internal_symbols


class WrapInternalSymbolsTest(unittest.TestCase):

    def test_wrap_internal_symbols(self):
        basename = 'test_wrap_internal_symbols'

        with open(f'tests/files/{basename}.o', 'rb') as fin:
            elf = fin.read()

        actual_wrapped = wrap_internal_symbols(elf, ['foo', 'bar'])

        with open(f'tests/files/{basename}.wrapped.o', 'rb') as fin:
            expected_wrapped = fin.read()

        self.assertEqual(actual_wrapped, expected_wrapped)

    def test_wrap_internal_symbols_function_sections(self):
        basename = 'test_wrap_internal_symbols_function_sections'

        with open(f'tests/files/{basename}.o', 'rb') as fin:
            elf = fin.read()

        actual_wrapped = wrap_internal_symbols(elf, ['foo', 'bar'])

        with open(f'tests/files/{basename}.wrapped.o', 'rb') as fin:
            expected_wrapped = fin.read()

        self.assertEqual(actual_wrapped, expected_wrapped)
