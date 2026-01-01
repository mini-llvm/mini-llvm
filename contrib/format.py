#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
from pathlib import Path
import re
import sys


def expand_tabs(content):
    return content.replace('\t', '    ')


def trim_trailing_whitespaces(content):
    return ''.join([line.rstrip(' ') + '\n' for line in content.splitlines()])


def trim_final_newlines(content):
    lines = content.splitlines()
    while lines and lines[-1] == '':
        lines.pop()
    return ''.join([line + '\n' for line in lines])


def sort_includes(content):
    result_lines = []
    lines = content.splitlines()
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


def sort_sources(content):
    replacements = []
    for command in re.finditer(r'add_(?:library|executable)\s*\(\s*[^)\s]+(?:\s+[A-Z]+)*((?:\s+[^)\s]+)*)\s*\)', content):
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
        content = content[:replacement[0]] + replacement[2] + content[replacement[1]:]
    return content


def reformat_cpp(content):
    content = expand_tabs(content)
    content = trim_trailing_whitespaces(content)
    content = trim_final_newlines(content)
    content = sort_includes(content)
    return content


def reformat_cmake(content):
    content = expand_tabs(content)
    content = trim_trailing_whitespaces(content)
    content = trim_final_newlines(content)
    content = sort_sources(content)
    return content


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', nargs='*')
    parser.add_argument('--check', action='store_true')

    args = parser.parse_args()
    exit_code = 0

    for path in args.input:
        path = Path(path)
        with open(path, mode='r', encoding='utf-8') as f:
            content = f.read()
        if path.suffix in ('.c', '.cpp', '.h'):
            reformatted_content = reformat_cpp(content)
        if path.suffix == '.cmake' or path.name == 'CMakeLists.txt':
            reformatted_content = reformat_cmake(content)
        if reformatted_content != content:
            if args.check:
                print(f'{path}: not properly formatted', file=sys.stderr)
                exit_code = 1
            else:
                with open(path, mode='w', encoding='utf-8') as f:
                    f.write(reformatted_content)
                print(f'{path}: reformatted')

    return exit_code


if __name__ == '__main__':
    sys.exit(main())
