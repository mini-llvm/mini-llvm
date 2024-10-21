#!/usr/bin/env python3
import argparse
import sys


def check_non_ascii_characters(source):
    return source.isascii()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', nargs='+')

    args = parser.parse_args()
    exit_code = 0

    for path in args.input:
        with open(path, mode='r', encoding='utf-8') as file:
            source = file.read()
        if not check_non_ascii_characters(source):
            print(f'{path}: non-ASCII characters are not allowed', file=sys.stderr)
            exit_code = 1

    return exit_code


if __name__ == '__main__':
    sys.exit(main())
