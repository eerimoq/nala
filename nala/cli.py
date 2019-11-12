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


def generate_main_c():
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'main.c'), 'main.c')


def generate_makefile():
    shutil.copyfile(os.path.join(TEMPLATES_DIR, 'Makefile'), 'Makefile')


def copy_nala():
    shutil.copyfile(os.path.join(DIST_DIR, 'nala.h'), 'nala.h')
    shutil.copyfile(os.path.join(DIST_DIR, 'nala.c'), 'nala.c')


def touch_mocks_h():
    with open('nala_mocks.h', 'w') as fout:
        pass


def do_init(args):
    generate_main_c()
    generate_makefile()
    copy_nala()
    touch_mocks_h()
    expanded_code = subprocess.check_output(['gcc', '-E', 'main.c'])
    generate_mocks(expanded_code.decode('utf-8'))

    print("Run 'make' to build and run the test suite!")


def do_generate_mocks(args):
    if not args.infiles:
        expanded_code = sys.stdin.read()
    else:
        expanded_code = ''

        for infile in infiles:
            with open(infile, 'r') as fin:
                expanded_code += fin.read()

    generate_mocks(expanded_code, args.outdir)


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
    subparser.set_defaults(func=do_init)

    subparser = subparsers.add_parser('generate_mocks',
                                      description='Generate mocks.')
    subparser.add_argument('-o', '--outdir',
                           default='.',
                           help='Output directory (default: %(default)s).')
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
