import unittest
from nala.wrap_internal_symbols import wrap_internal_symbols


class WrapInternalSymbolsTest(unittest.TestCase):

    def test_wrap_internal_symbols(self):
        with open('tests/files/test_wrap_internal_symbols.o', 'rb') as fin:
            elf = fin.read()

        actual_wrapped = wrap_internal_symbols(elf, ['foo', 'bar'])

        with open('tests/files/test_wrap_internal_symbols.wrapped.o', 'rb') as fin:
            expected_wrapped = fin.read()

        self.assertEqual(actual_wrapped, expected_wrapped)
