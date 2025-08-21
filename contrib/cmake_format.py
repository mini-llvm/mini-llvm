#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
import re
import sys


def expand_tabs(script):
    return script.replace('\t', '    ')


def trim_trailing_whitespaces(script):
    return ''.join([line.rstrip(' ') + '\n' for line in script.splitlines()])


def trim_final_newlines(script):
    lines = script.splitlines()
    while lines and lines[-1] == '':
        lines.pop()
    return ''.join([line + '\n' for line in lines])


def sort_sources(script):
    replacements = []
    for command in re.finditer(r'add_(?:library|executable)\s*\(\s*[^)\s]+(?:\s+[A-Z]+)*((?:\s+[^)\s]+)*)\s*\)', script):
        if command.group(1):
            sources = []
            for source in re.finditer(r'[^)\s]+', command.group(1)):
                sources.append((command.start(1) + source.start(0), command.start(1) + source.end(0), source.group(0)))
            sources = list(zip(*sources))
            sources[2] = sorted(sources[2], key=str.lower)
            sources = list(zip(*sources))
            for i in reversed(range(0, len(sources) - 1)):
                if sources[i][2] == sources[i + 1][2]:
                    sources = sources[:i] + [(sources[i][0], sources[i + 1][1], sources[i + 1][2])] + sources[i + 2:]
            replacements.extend(sources)
    for replacement in reversed(replacements):
        script = script[:replacement[0]] + replacement[2] + script[replacement[1]:]
    return script


def reformat(script):
    script = expand_tabs(script)
    script = trim_trailing_whitespaces(script)
    script = trim_final_newlines(script)
    script = sort_sources(script)
    return script


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', nargs='*')
    parser.add_argument('--check', action='store_true')

    args = parser.parse_args()
    exit_code = 0

    for path in args.input:
        with open(path, mode='r', encoding='utf-8') as f:
            script = f.read()
        reformatted_script = reformat(script)
        if reformatted_script != script:
            if args.check:
                print(f'{path}: not properly formatted', file=sys.stderr)
                exit_code = 1
            else:
                with open(path, mode='w', encoding='utf-8') as f:
                    f.write(reformatted_script)
                print(f'{path}: reformatted')

    return exit_code


if __name__ == '__main__':
    sys.exit(main())
