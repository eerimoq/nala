import os
import sys
import argparse
import shutil
import subprocess

from .api import generate_mocks
from .version import __version__


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
DIST_DIR = os.path.join(SCRIPT_DIR, 'dist')
TEMPLATES_DIR = os.path.join(SCRIPT_DIR, 'templates')
RENAME_PARAMETERS_TXT = os.path.join(SCRIPT_DIR, 'rename_parameters.txt')


def generate_suites():
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'test_assertions.c'),
                    'test_assertions.c')
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'test_time.c'),
                    'test_time.c')


def generate_makefile():
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'Makefile'), 'Makefile')


def copy_nala():
    shutil.copyfile(os.path.join(DIST_DIR, 'nala.h'), 'nala.h')
    shutil.copyfile(os.path.join(DIST_DIR, 'nala.c'), 'nala.c')


def do_init(args):
    os.mkdir(args.name)
    os.chdir(args.name)
    generate_suites()
    generate_makefile()
    copy_nala()

    print(f"Run 'make -C {args.name}' to build and run the test suite!")


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

    generate_mocks(expanded_code,
                   args.outdir,
                   rename_parameters_file,
                   not args.no_cache)


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

    subparser = subparsers.add_parser(
        'init',
        description='Create a test suite in current directory.')
    subparser.add_argument('name', help='Test suite name.')
    subparser.set_defaults(func=do_init)

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
    subparser.add_argument('-c', '--no-cache',
                           action='store_true',
                           help='Do not use caching to speed up the generation.')
    subparser.add_argument(
        'infiles',
        nargs='*',
        help='Pre-processed source file(s) or - to read from stdin (default: -).')
    subparser.set_defaults(func=do_generate_mocks)

    args = parser.parse_args()

    if args.debug:
        args.func(args)
    else:
        try:
            args.func(args)
        except BaseException as e:
            sys.exit('error: ' + str(e))
