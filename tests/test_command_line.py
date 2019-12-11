import unittest
from unittest.mock import patch
from io import StringIO
import nala.cli


def read_file(filename):
    with open(filename, 'r') as fin:
        return fin.read()


class CommandLineTest(unittest.TestCase):

    def assert_files_equal(self, actual, expected):
        # open(expected, 'w').write(read_file(actual))
        self.assertEqual(read_file(actual), read_file(expected))

    def assert_generated_files(self, name):
        self.assert_files_equal('output/nala_mocks.h',
                                f'tests/files/test_{name}_nala_mocks.h')
        self.assert_files_equal('output/nala_mocks.c',
                                f'tests/files/test_{name}_nala_mocks.c')
        self.assert_files_equal('output/nala_mocks.ld',
                                f'tests/files/test_{name}_nala_mocks.ld')

    def test_generate_mocks_empty(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/test_empty_tests.pp.c'
        ]

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('empty')

    def test_generate_mocks_collect(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/test_collect_tests.pp.c'
        ]

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('collect')
