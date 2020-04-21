import os
import re
import subprocess
import unittest
from unittest.mock import patch
import shutil
import filecmp
from io import StringIO
import nala.cli


def remove_date_time(string):
    return re.sub(r'Generated with Nala.*', '', string)


def remove_assert_in(string):
    return re.sub(r'// Struct assertions.*', '', string, flags=re.DOTALL)


def read_file(filename):
    with open(filename, 'r') as fin:
        return remove_assert_in(remove_date_time(fin.read()))


def remove_optput():
    if os.path.exists('output'):
        shutil.rmtree('output')


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
        self.assert_files_equal('output/nala_mocks.ldflags',
                                f'tests/files/test_{name}_nala_mocks.ldflags')

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

            remove_optput()
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
            'tests/files/test_missing_declaration_tests.pp.c'
        ]

        remove_optput()
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

    def test_generate_mocks_open_no_real_variadic(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            '--no-real-variadic-functions',
            'tests/files/test_open_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('open')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('open')

    def test_generate_mocks_rename_underscores_tests(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/test_rename_underscores_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('rename_underscores')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('rename_underscores')

    def test_generate_mocks_no_rename_underscores_tests(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            '--no-rename-parameters',
            'tests/files/test_no_rename_underscores_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('no_rename_underscores')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('no_rename_underscores')

    def test_wrap_internal_symbols(self):
        argv = [
            'nala',
            'wrap_internal_symbols',
            'tests/files/test_wrap_internal_symbols.ldflags',
            'test_wrap_internal_symbols.o'
        ]

        # The file is modified in place, so make a copy.
        shutil.copyfile('tests/files/test_wrap_internal_symbols.o',
                        'test_wrap_internal_symbols.o')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assertTrue(
            filecmp.cmp('test_wrap_internal_symbols.o',
                        'tests/files/test_wrap_internal_symbols.wrapped.o'))

    def test_regenerate_mocks_cache_no_change(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/test_dummy_functions_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('dummy_functions')

        with patch('sys.argv', argv):
            nala.cli.main()

        # Set file date to 1970.
        os.utime('output/nala_mocks.h', (0, 0))
        os.utime('output/nala_mocks.c', (0, 0))
        os.utime('output/nala_mocks.ldflags', (0, 0))

        with patch('sys.argv', argv):
            nala.cli.main()

        # No regeneration should have happened.
        self.assertEqual(os.stat('output/nala_mocks.h').st_mtime, 0)
        self.assertEqual(os.stat('output/nala_mocks.c').st_mtime, 0)
        self.assertEqual(os.stat('output/nala_mocks.ldflags').st_mtime, 0)

    def test_regenerate_mocks_no_cache_no_change(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            '--no-cache',
            'tests/files/test_dummy_functions_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('dummy_functions')

        with patch('sys.argv', argv):
            nala.cli.main()

        # Set file date to 1970.
        os.utime('output/nala_mocks.h', (0, 0))
        os.utime('output/nala_mocks.c', (0, 0))
        os.utime('output/nala_mocks.ldflags', (0, 0))

        with patch('sys.argv', argv):
            nala.cli.main()

        # Regeneration happened as --no-cache was given.
        self.assertNotEqual(os.stat('output/nala_mocks.h').st_mtime, 0)
        self.assertNotEqual(os.stat('output/nala_mocks.c').st_mtime, 0)
        self.assertNotEqual(os.stat('output/nala_mocks.ldflags').st_mtime, 0)

    def test_generate_mocks_touched_nala_mocks_h_and_empty_test(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/test_empty_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('empty')

        # Touch output/nala_mocks.h before generating, just as test.mk
        # does.
        os.mkdir('output')

        with open('output/nala_mocks.h', 'w'):
            pass

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('empty')
