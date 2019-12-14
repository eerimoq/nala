import time
import subprocess
import unittest
from nala.cparser import parse


def pre_process_file(name):
    filename = f'tests/files/test_{name}_tests.pp.c'
    subprocess.run([
        'gcc',
        '-E',
        '-I', 'nala/dist',
        '-o', filename,
        f'tests/files/test_{name}_tests.c'
    ])

    with open(filename, 'r') as fin:
        return fin.read()


class CParserTest(unittest.TestCase):

    def test_parse_empty(self):
        parsed = parse('')
        self.assertEqual(parsed, [])

    def test_parse_linemarker(self):
        parsed = parse('# 1 "/usr/include/stdc-predef.h" 1 3 4')

        self.assertEqual(
            parsed,
            [
                '# 1 "/usr/include/stdc-predef.h" 1 3 4'
            ])

    def test_parse_typedef_primitive(self):
        parsed = parse('typedef long unsigned int size_t;')

        self.assertEqual(
            parsed,
            [
                ['typedef', [['long', 'unsigned', 'int', 'size_t'], [], ';']]
            ])

    def test_parse_typedef_struct(self):
        parsed = parse('typedef struct {\n'
                       '    int a;\n'
                       '} foo_t;')

        self.assertEqual(
            parsed,
            [
                ['typedef', ['struct', ['{', '}'], 'foo_t', [], ';']]
            ])

    def test_parse_function_declaration(self):
        parsed = parse('void foo();')

        self.assertEqual(
            parsed,
            [
                ('function', [[], ['void', 'foo'], ['(', [], ')'], [], ';'])
            ])

    def test_parse_function_declaration(self):
        parsed = parse('void foo(int (*a)());')

        self.assertEqual(
            parsed,
            [
                ('function', [[], ['void', 'foo'], ['(',  ')'], [], ';'])
            ])

    def test_parse_dummy_functions(self):
        string = pre_process_file('dummy_functions')
        start_time = time.time()
        parse(string)
        print('Measured time:', round(time.time() - start_time, 2))
