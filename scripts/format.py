#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

_BUILDIFIER = os.getenv('BUILDIFIER', 'buildifier')

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


def is_cpp(path):
    return path.suffix in ('.cpp', '.h')


def is_cmake(path):
    if path.suffix == '.cmake':
        return True
    if path.name == 'CMakeLists.txt':
        return True
    return False


def is_bazel(path):
    if path.suffix == '.bzl':
        return True
    if path.name in ('BUILD', 'BUILD.bazel', 'WORKSPACE', 'WORKSPACE.bazel', 'MODULE.bazel'):
        return True
    return False


def reformat_cpp_content(content):
    content = expand_tabs(content)
    content = trim_trailing_whitespaces(content)
    content = trim_final_newlines(content)
    content = sort_includes(content)
    return content


def reformat_cmake_content(content):
    content = expand_tabs(content)
    content = trim_trailing_whitespaces(content)
    content = trim_final_newlines(content)
    content = sort_sources(content)
    return content


def reformat_cpp(path):
    with open(path, mode='r', encoding='utf-8') as f:
        content = f.read()
    reformatted_content = reformat_cpp_content(content)
    if reformatted_content == content:
        return False
    with open(path, mode='w', encoding='utf-8') as f:
        f.write(reformatted_content)
    return True


def reformat_cmake(path):
    with open(path, mode='r', encoding='utf-8') as f:
        content = f.read()
    reformatted_content = reformat_cmake_content(content)
    if reformatted_content == content:
        return False
    with open(path, mode='w', encoding='utf-8') as f:
        f.write(reformatted_content)
    return True


def reformat_bazel(path):
    with open(path, mode='r', encoding='utf-8') as f:
        content = f.read()
    subprocess.run([_BUILDIFIER, str(path)], check=True)
    with open(path, mode='r', encoding='utf-8') as f:
        reformatted_content = f.read()
    return reformatted_content != content


def check_cpp(path):
    with open(path, mode='r', encoding='utf-8') as f:
        content = f.read()
    return content == reformat_cpp_content(content)


def check_cmake(path):
    with open(path, mode='r', encoding='utf-8') as f:
        content = f.read()
    return content == reformat_cmake_content(content)


def check_bazel(path):
    result = subprocess.run([_BUILDIFIER, '-mode=check', str(path)])
    if result.returncode == 0:
        return True
    if result.returncode == 4:
        return False
    raise subprocess.CalledProcessError(result.returncode, result.args)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', nargs='*')
    parser.add_argument('--check', action='store_true')

    args = parser.parse_args()

    paths = [Path(path) for path in args.input]

    if any(is_bazel(path) for path in paths) and shutil.which(_BUILDIFIER) is None:
        print(
            'error: could not find buildifier\n'
            'buildifier is required to format Bazel files. Install it with:\n'
            '  brew: brew install buildifier\n'
            '    go: go install github.com/bazelbuild/buildtools/buildifier@latest',
            file=sys.stderr,
        )
        return 1

    exit_code = 0

    for path in paths:
        if args.check:
            if is_cpp(path):
                ok = check_cpp(path)
            elif is_cmake(path):
                ok = check_cmake(path)
            elif is_bazel(path):
                ok = check_bazel(path)
            else:
                ok = True
            if not ok:
                print(f'{path}: not properly formatted', file=sys.stderr)
                exit_code = 1
        else:
            if is_cpp(path):
                reformatted = reformat_cpp(path)
            elif is_cmake(path):
                reformatted = reformat_cmake(path)
            elif is_bazel(path):
                reformatted = reformat_bazel(path)
            else:
                reformatted = False
            if reformatted:
                print(f'{path}: reformatted')

    return exit_code


if __name__ == '__main__':
    sys.exit(main())
