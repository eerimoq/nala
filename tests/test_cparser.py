import re
import time
import subprocess
import unittest
from nala.cparser import parse
from nala.cparser import IncludeFinder
from textparser import Token


def pre_process_file(name):
    source_path = f'tests/files/test_{name}_tests.c'
    expanded_source_path = f'tests/files/test_{name}_tests.pp.c'
    subprocess.run([
        'gcc',
        '-E',
        '-I', 'nala/dist',
        '-o', expanded_source_path,
        source_path
    ])

    with open(source_path, 'r') as fin:
        source = fin.read()

    with open(expanded_source_path, 'r') as fin:
        expanded_source = fin.read()

    return source, expanded_source


class CParserTest(unittest.TestCase):

    def test_parse_empty(self):
        parsed = parse('', '', set())
        self.assertEqual(parsed.functions, {})

    def test_parse_dummy_functions(self):
        source, expanded_source = pre_process_file('dummy_functions')
        start_time = time.time()
        parsed = parse(source, expanded_source, set(['malloc',
                                                     'mount',
                                                     'struct_param']))
        print('Measured time:', round(time.time() - start_time, 2))

        from pprint import pprint
        pprint(parsed.includes)
        pprint(list(parsed.functions.keys()))
        pprint(list(parsed.structs.keys()))
