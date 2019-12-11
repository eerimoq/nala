import subprocess
import unittest
from unittest.mock import patch
from io import StringIO
import nala.cli


def read_file(filename):
    with open(filename, 'r') as fin:
        return fin.read()


def pre_process_file(name):
    subprocess.run([
        'gcc',
        '-E',
        '-I', 'nala/dist',
        '-o', f'tests/files/test_{name}_tests.pp.c',
        f'tests/files/test_{name}_tests.c'
    ])


class CommandLineTest(unittest.TestCase):

    maxDiff = None

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

    def test_generate_mocks(self):
        names = [
            'empty',
            'collect',
            'dummy_functions'
        ]

        for name in names:
            argv = [
                'nala',
                'generate_mocks',
                '-o', 'output',
                f'tests/files/test_{name}_tests.pp.c'
            ]

            pre_process_file(name)

            with patch('sys.argv', argv):
                nala.cli.main()

            self.assert_generated_files(name)
