import os
import sys
import argparse
import shutil
import re

from .wrap_internal_symbols import wrap_internal_symbols
from .version import __version__


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
DIST_DIR = os.path.join(SCRIPT_DIR, 'dist')
SUBCOMMANDS_INIT_DIR = os.path.join(SCRIPT_DIR, 'subcommands', 'init')
RENAME_PARAMETERS_TXT = os.path.join(SCRIPT_DIR, 'rename_parameters.txt')
REAL_VARIADIC_FUNCTIONS_C = os.path.join(SCRIPT_DIR, 'real_variadic_functions.c')


def do_init(args):
    shutil.copytree(SUBCOMMANDS_INIT_DIR, args.name)

    print(f"Run 'make -C {args.name}' to build and run all tests!")


def do_generate_mocks(args):
    if not args.infile:
        expanded_code = sys.stdin.read()
    else:
        with open(args.infile, 'r') as fin:
            expanded_code = fin.read()

    if args.no_rename_parameters:
        rename_parameters_file = None
    else:
        rename_parameters_file = args.rename_parameters_file

    if args.no_real_variadic_functions:
        real_variadic_functions_file = None
    else:
        real_variadic_functions_file = REAL_VARIADIC_FUNCTIONS_C

    # Only import when needed for faster cli.
    from .api import generate_mocks

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


def do_include_dir(args):
    print(DIST_DIR)


def do_c_sources(args):
    print(os.path.join(DIST_DIR, 'nala.c'))


def do_cat(args):
    for cfile in args.cfiles:
        with open(cfile, 'r') as fin:
            print(f'#line 1 "{cfile}"')
            sys.stdout.write(fin.read())


def main():
    parser = argparse.ArgumentParser(
        prog='nala',
        description='A minimal mocking utility for C projects.')

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
        'infile',
        nargs='?',
        help='Pre-processed source file or - to read from stdin (default: -).')
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

    # The include_dir subcommand.
    subparser = subparsers.add_parser(
        'include_dir',
        description='Print the path to the C include directory.')
    subparser.set_defaults(func=do_include_dir)

    # The c_sources subcommand.
    subparser = subparsers.add_parser(
        'c_sources',
        description='Print the C sources.')
    subparser.set_defaults(func=do_c_sources)

    # The cat subcommand.
    subparser = subparsers.add_parser(
        'cat',
        description=('Concatenate given files with pre-processor line control '
                     'for correct error location information.'))
    subparser.add_argument('cfiles',
                           nargs='+',
                           help='One of more C source files to concatename.')
    subparser.set_defaults(func=do_cat)

    args = parser.parse_args()

    if args.debug:
        args.func(args)
    else:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit('error: ' + str(e))
