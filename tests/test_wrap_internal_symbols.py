import unittest
from nala.wrap_internal_symbols import wrap_internal_symbols


class WrapInternalSymbolsTest(unittest.TestCase):

    def test_wrap_internal_symbols(self):
        path = 'wrap_internal_symbols/test'

        with open(f'tests/files/{path}.o', 'rb') as fin:
            elf = fin.read()

        actual_wrapped = wrap_internal_symbols(elf, ['foo', 'bar'])

        with open(f'tests/files/{path}.wrapped.o', 'rb') as fin:
            expected_wrapped = fin.read()

        self.assertEqual(actual_wrapped, expected_wrapped)

    def test_wrap_internal_symbols_function_sections(self):
        path = 'wrap_internal_symbols/test_function_sections'

        with open(f'tests/files/{path}.o', 'rb') as fin:
            elf = fin.read()

        actual_wrapped = wrap_internal_symbols(elf, ['foo', 'bar'])

        with open(f'tests/files/{path}.wrapped.o', 'rb') as fin:
            expected_wrapped = fin.read()

        self.assertEqual(actual_wrapped, expected_wrapped)

    def test_wrap_internal_symbols_unmodified(self):
        path = 'wrap_internal_symbols/test'

        with open(f'tests/files/{path}.o', 'rb') as fin:
            elf = fin.read()

        self.assertIsNone(wrap_internal_symbols(elf, ['not', 'found']))
