import sys
from argparse import ArgumentParser, FileType

from .api import generate_mocks, collect_linker_flags


def main():
    parser = ArgumentParser(
        prog="nala",
        description="A minimal mocking utility for C projects.")

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-g",
                       metavar="<code>",
                       nargs="?",
                       type=FileType("r"),
                       const=sys.stdin,
                       help="generate mocks")
    group.add_argument("-f",
                       action="store_true",
                       help="output linker flags")

    parser.add_argument("-d",
                        metavar="<directory>",
                        default=".",
                        help="mocks directory")
    args = parser.parse_args()

    if args.g:
        generate_mocks(expanded_code=args.g.read(), directory=args.d)
    elif args.f:
        print(" ".join(collect_linker_flags(directory=args.d)).strip())
