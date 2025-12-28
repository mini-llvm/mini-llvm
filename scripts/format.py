#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
from pathlib import Path
import re
import sys

_INCLUDE_PATTERN = re.compile(r'#\s*include\s*[<"]\s*([^\s>"]+)\s*[>"]')
_COMMAND_PATTERN = re.compile(r'\badd_(?:library|executable)\s*\(\s*[^\s)]+(?:\s+[A-Z]+)*((?:\s+[^)\s]+)*)\s*\)')
_SOURCE_PATTERN = re.compile(r'[^\s)]+')


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
    lines = content.splitlines()
    n = len(lines)
    i = 0
    while i < n:
        if _INCLUDE_PATTERN.search(lines[i]):
            j = i
            while j + 1 < n and _INCLUDE_PATTERN.search(lines[j + 1]):
                j += 1
            lines[i : j + 1] = sorted(lines[i : j + 1], key=lambda line: _INCLUDE_PATTERN.search(line).group(1).lower())
            i = j + 1
        else:
            i += 1
    return ''.join([line + '\n' for line in lines])


def sort_sources(content):
    replacements = []
    for command in _COMMAND_PATTERN.finditer(content):
        if command.group(1):
            sources = []
            for source in _SOURCE_PATTERN.finditer(command.group(1)):
                sources.append((command.start(1) + source.start(0), command.start(1) + source.end(0), source.group(0)))
            sources = list(zip(*sources))
            sources[2] = sorted(sources[2], key=str.lower)
            sources = list(zip(*sources))
            replacements.extend(sources)
    for replacement in reversed(replacements):
        content = content[: replacement[0]] + replacement[2] + content[replacement[1] :]
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
        elif path.suffix == '.cmake':
            reformatted_content = reformat_cmake(content)
        elif path.name == 'CMakeLists.txt':
            reformatted_content = reformat_cmake(content)
        else:
            reformatted_content = content
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
