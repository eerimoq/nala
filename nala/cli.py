import os
import sys
import argparse
import shutil
import re

from .api import generate_mocks
from .wrap_internal_symbols import wrap_internal_symbols
from .version import __version__


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
DIST_DIR = os.path.join(SCRIPT_DIR, 'dist')
TEMPLATES_DIR = os.path.join(SCRIPT_DIR, 'templates')
RENAME_PARAMETERS_TXT = os.path.join(SCRIPT_DIR, 'rename_parameters.txt')
REAL_VARIADIC_FUNCTIONS_C = os.path.join(SCRIPT_DIR, 'real_variadic_functions.c')


def generate_suites():
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'test_assertions.c'),
                    'test_assertions.c')
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'test_time.c'),
                    'test_time.c')


def generate_makefiles():
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'Makefile'), 'Makefile')
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'test.mk'), 'test.mk')


def copy_nala():
    shutil.copyfile(os.path.join(DIST_DIR, 'nala.h'), 'nala.h')
    shutil.copyfile(os.path.join(DIST_DIR, 'nala.c'), 'nala.c')


def do_init(args):
    os.mkdir(args.name)
    os.chdir(args.name)
    generate_suites()
    generate_makefiles()
    copy_nala()

    print(f"Run 'make -C {args.name}' to build and run all tests!")


def do_generate_mocks(args):
    if not args.infiles:
        expanded_code = sys.stdin.read()
    else:
        expanded_code = ''

        for infile in args.infiles:
            with open(infile, 'r') as fin:
                expanded_code += fin.read()

    if args.no_rename_parameters:
        rename_parameters_file = None
    else:
        rename_parameters_file = args.rename_parameters_file

    if args.no_real_variadic_functions:
        real_variadic_functions_file = None
    else:
        real_variadic_functions_file = REAL_VARIADIC_FUNCTIONS_C

    generate_mocks(expanded_code,
                   args.outdir,
                   rename_parameters_file,
                   real_variadic_functions_file,
                   not args.no_cache,
                   args.implementation,
                   args.no_implementation)


def do_wrap_internal_symbols(args):
    with open(args.ldflagsfile, 'r') as fin:
        flags = fin.read().split(' ')

    symbols_to_wrap = []

    for flag in flags:
        mo = re.match(r'-Wl,--wrap=(.*)', flag)

        if mo:
            symbols_to_wrap.append(mo.group(1))

    with open(args.objectfile, 'rb') as fin:
        wrapped_data = wrap_internal_symbols(fin.read(), symbols_to_wrap)

    if wrapped_data is None:
        return

    with open(args.objectfile, 'wb') as fout:
        fout.write(wrapped_data)


def main():
    parser = argparse.ArgumentParser(
        prog="nala",
        description="A minimal mocking utility for C projects.")

    parser.add_argument('-d', '--debug', action='store_true')
    parser.add_argument('--version',
                        action='version',
                        version=__version__,
                        help='Print version information and exit.')

    # Workaround to make the subparser required in Python 3.
    subparsers = parser.add_subparsers(title='subcommands',
                                       dest='subcommand')
    subparsers.required = True

    # The init subcommand.
    subparser = subparsers.add_parser(
        'init',
        description='Create a test suite in current directory.')
    subparser.add_argument('name', help='Test suite name.')
    subparser.set_defaults(func=do_init)

    # The generate_mocks subparser.
    subparser = subparsers.add_parser('generate_mocks',
                                      description='Generate mocks.')
    subparser.add_argument('-o', '--outdir',
                           default='.',
                           help='Output directory (default: %(default)s).')
    subparser.add_argument('-r', '--rename-parameters-file',
                           default=RENAME_PARAMETERS_TXT,
                           help='Rename parameters file (default: %(default)s).')
    subparser.add_argument('-R', '--no-rename-parameters',
                           action='store_true',
                           help='Do not rename any parameters.')
    subparser.add_argument('-F', '--no-real-variadic-functions',
                           action='store_true',
                           help='Do not add any real variadic functions.')
    subparser.add_argument('-c', '--no-cache',
                           action='store_true',
                           help='Do not use caching to speed up the generation.')
    subparser.add_argument('-i', '--implementation',
                           action='append',
                           default=[],
                           help=('Call functions matching given Unix shell-style '
                                 'wildcards pattern. Overrides --no-implementation. '
                                 'May be given multiple times.'))
    subparser.add_argument('-n', '--no-implementation',
                           action='append',
                           default=[],
                           help=('Do not call functions matching given Unix '
                                 'shell-style wildcards pattern. May be given '
                                 'multiple times.'))
    subparser.add_argument(
        'infiles',
        nargs='*',
        help='Pre-processed source file(s) or - to read from stdin (default: -).')
    subparser.set_defaults(func=do_generate_mocks)

    # The wrap_internal_symbols subparser.
    subparser = subparsers.add_parser(
        'wrap_internal_symbols',
        description='Wrap given object-internal symbols.')
    subparser.add_argument('ldflagsfile',
                           help='The generated .ldflags file.')
    subparser.add_argument('objectfile',
                           help='An object file.')
    subparser.set_defaults(func=do_wrap_internal_symbols)

    args = parser.parse_args()

    if args.debug:
        args.func(args)
    else:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit('error: ' + str(e))
