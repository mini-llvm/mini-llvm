#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
import sys


def expand_tabs(source):
    return source.replace('\t', '    ')


def trim_trailing_whitespaces(source):
    return ''.join([line.rstrip(' ') + '\n' for line in source.splitlines()])


def trim_final_newlines(source):
    lines = source.splitlines()
    while lines and lines[-1] == '':
        lines.pop()
    return ''.join([line + '\n' for line in lines])


def sort_includes(source):
    result_lines = []
    lines = source.splitlines()
    n = len(lines)
    i = 0
    while i < n:
        if lines[i].startswith('#include'):
            j = i
            while j + 1 < n and lines[j + 1].startswith('#include'):
                j += 1
            result_lines.extend(sorted(set(lines[i : j + 1]), key=str.lower))
            i = j + 1
        else:
            result_lines.append(lines[i])
            i += 1
    return ''.join([line + '\n' for line in result_lines])


def reformat(source):
    source = expand_tabs(source)
    source = trim_trailing_whitespaces(source)
    source = trim_final_newlines(source)
    source = sort_includes(source)
    return source


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', nargs='*')
    parser.add_argument('--check', action='store_true')

    args = parser.parse_args()
    exit_code = 0

    for path in args.input:
        with open(path, mode='r', encoding='utf-8') as f:
            source = f.read()
        reformatted_source = reformat(source)
        if reformatted_source != source:
            if args.check:
                print(f'{path}: not properly formatted', file=sys.stderr)
                exit_code = 1
            else:
                with open(path, mode='w', encoding='utf-8') as f:
                    f.write(reformatted_source)
                print(f'{path}: reformatted')

    return exit_code


if __name__ == '__main__':
    sys.exit(main())
