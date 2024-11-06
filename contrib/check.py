#!/usr/bin/env python3
import argparse
import re
import sys


def check(source):
    errors = []

    for char in source:
        if not char.isascii():
            errors.append(f'{repr(char)} not allowed')

    return errors


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', nargs='+')

    args = parser.parse_args()
    exit_code = 0

    for path in args.input:
        with open(path, mode='r', encoding='utf-8') as file:
            source = file.read()
        errors = check(source)
        if errors:
            for error in errors:
                print(f'{path}: {error}', file=sys.stderr)
            exit_code = 1

    return exit_code


if __name__ == '__main__':
    sys.exit(main())
