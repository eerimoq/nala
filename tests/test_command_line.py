import re
import subprocess
import unittest
from unittest.mock import patch
from io import StringIO
import nala.cli


def remove_date_time(string):
    return re.sub(r'Generated with Nala.*', '', string)


def remove_assert_in(string):
    return re.sub(r'// Struct assertions.*', '', string, flags=re.DOTALL)


def read_file(filename):
    with open(filename, 'r') as fin:
        return remove_assert_in(remove_date_time(fin.read()))


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

    def test_generate_mocks_missing_declaration(self):
        argv = [
            'nala',
            '-d',
            'generate_mocks',
            '-o', 'output',
            f'tests/files/test_missing_declaration_tests.pp.c'
        ]

        pre_process_file('missing_declaration')
        stderr = StringIO()

        with patch('sys.argv', argv):
            with patch('sys.stderr', stderr):
                with self.assertRaises(Exception) as cm:
                    nala.cli.main()

        self.assertEqual(stderr.getvalue(),
                         "error: Mocked function 'bar' undeclared. Missing include?\n"
                         "error: Mocked function 'fie' undeclared. Missing include?\n"
                         "error: Mocked function 'foo' undeclared. Missing include?\n"
                         "error: Mocked function 'fum' undeclared. Missing include?\n"
                         "error: Mocked function 'gam' undeclared. Missing include?\n"
                         "error: Mocked function 'hit' undeclared. Missing include?\n")
        self.assertEqual(
            str(cm.exception),
            'Unable to find declarations of all mocked functions. Add '
            'missing includes to the test file.')
