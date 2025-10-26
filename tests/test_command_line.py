import os
import re
import subprocess
import platform
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
    command = [
        'gcc',
        '-std=c17',	# this avoids gcc to use gnu extensions, which are not supported by pycparser
        '-E',
        '-I', 'nala/dist',
        '-o', f'tests/files/{name}/test_tests.pp.c',
        f'tests/files/{name}/test_tests.c'
    ]

    subprocess.run(command, check=True)


class CommandLineTest(unittest.TestCase):

    maxDiff = None

    def assert_files_equal(self, actual, expected):
        # open(expected, 'w').write(read_file(actual))
        self.assertEqual(read_file(actual), read_file(expected))

    def assert_generated_files(self, name):
        self.assert_files_equal('output/nala_mocks.h',
                                f'tests/files/{name}/test_nala_mocks.h')
        self.assert_files_equal('output/nala_mocks.c',
                                f'tests/files/{name}/test_nala_mocks.c')
        self.assert_files_equal('output/nala_mocks.ldflags',
                                f'tests/files/{name}/test_nala_mocks.ldflags')

    def test_generate_mocks_linux(self):
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
                f'tests/files/{name}/test_tests.pp.c'
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
            'tests/files/missing_declaration/test_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('missing_declaration')
        stderr = StringIO()

        with patch('sys.argv', argv):
            with patch('sys.stderr', stderr):
                with self.assertRaises(Exception) as cm:
                    nala.cli.main()

        self.assertEqual(
            stderr.getvalue(),
            "error: Mocked function 'bar' undeclared. Add missing include in "
            "the test file.\n"
            "error: Mocked function 'fie' undeclared. Add missing include in "
            "the test file.\n"
            "error: Mocked function 'foo' undeclared. Add missing include in "
            "the test file.\n"
            "error: Mocked function 'fum' undeclared. Add missing include in "
            "the test file.\n"
            "error: Mocked function 'gam' undeclared. Add missing include in "
            "the test file.\n"
            "error: Mocked function 'hit' undeclared. Add missing include in "
            "the test file.\n")
        self.assertEqual(
            str(cm.exception),
            'Unable to find declarations of all mocked functions. Add '
            'missing include(s) in the test file.')

    def test_generate_mocks_open_no_real_variadic(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            '--no-real-variadic-functions',
            'tests/files/open/test_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('open')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('open')

    def test_generate_mocks_rename_parameters_without_name(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            '-r', 'tests/files/rename_parameters_without_name/test.txt',
            'tests/files/rename_parameters_without_name/test_tests.pp.c'
        ]

        remove_optput()
        pre_process_file('rename_parameters_without_name')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assert_generated_files('rename_parameters_without_name')

    def test_generate_mocks_rename_underscores_tests(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/rename_underscores/test_tests.pp.c'
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
            'tests/files/no_rename_underscores/test_tests.pp.c'
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
            'tests/files/wrap_internal_symbols/test.ldflags',
            'wrap_internal_symbols_test.o'
        ]

        # The file is modified in place, so make a copy.
        shutil.copyfile('tests/files/wrap_internal_symbols/test.o',
                        'wrap_internal_symbols_test.o')

        with patch('sys.argv', argv):
            nala.cli.main()

        self.assertTrue(
            filecmp.cmp('wrap_internal_symbols_test.o',
                        'tests/files/wrap_internal_symbols/test.wrapped.o'))

    def test_regenerate_mocks_cache_no_change(self):
        argv = [
            'nala',
            'generate_mocks',
            '-o', 'output',
            'tests/files/dummy_functions/test_tests.pp.c'
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
            'tests/files/dummy_functions/test_tests.pp.c'
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
            'tests/files/empty/test_tests.pp.c'
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

    def test_cat(self):
        argv = [
            'nala',
            'cat',
            'tests/files/empty/test_tests.c',
            'tests/files/pre_processor_error/test_tests.c'
        ]

        stdout = StringIO()

        with patch('sys.argv', argv):
            with patch('sys.stdout', stdout):
                nala.cli.main()

        remove_optput()
        os.mkdir('output')

        with open('output/tests.c', 'w') as fout:
            fout.write(stdout.getvalue())

        command = [
            'gcc',
            '-E',
            '-I', 'nala/dist',
            '-o', 'output/tests.pp.c',
            'output/tests.c'
        ]
        stderr = subprocess.run(command,
                                stderr=subprocess.PIPE,
                                encoding='utf-8').stderr
        self.assertIn(
            'tests/files/pre_processor_error/test_tests.c:4:10: error: '
            'macro "FOO" passed 1 arguments, but takes just 0',
            stderr)
        self.assertIn('int FOO(1);', stderr)
