import unittest
import nala.linker


class LinkerTest(unittest.TestCase):

    def test_wrap_symbols(self):
        with open('tests/files/foo.o', 'rb') as fin:
            elf = fin.read()

        actual_wrapped = nala.linker.wrap_symbols(elf, ['foo', 'bar'])

        # with open('tests/files/foo.actual.o', 'wb') as fout:
        #     fout.write(actual_wrapped)

        with open('tests/files/foo.wrapped.o', 'rb') as fin:
            expected_wrapped = fin.read()

        self.assertEqual(actual_wrapped, expected_wrapped)
