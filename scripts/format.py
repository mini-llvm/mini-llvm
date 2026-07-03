#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
import os
import re
import shutil
import subprocess
import sys
from abc import ABC, abstractmethod
from itertools import groupby
from pathlib import Path

_INCLUDE_PATTERN = re.compile(r'^\s*#\s*include\s*[<"]\s*([^\s>"]+)\s*[>"]')
_COMMAND_PATTERN = re.compile(
    r"\badd_(?:library|executable)\s*\(\s*[^\s)]+(?:\s+[A-Z_]+)*((?:\s+[^)\s]+)*)\s*\)"
)
_SOURCE_PATTERN = re.compile(r"[^\s)]+")

_BUILDIFIER = os.getenv("BUILDIFIER", "buildifier")


class _FormatError(Exception):
    pass


class _Formatter(ABC):
    @abstractmethod
    def reformat(self, path):
        pass

    @abstractmethod
    def check(self, path):
        pass


def _expand_tabs(content, tab_width=4):
    return content.replace("\t", " " * tab_width)


def _trim_trailing_whitespace(content):
    return "".join([line.rstrip() + "\n" for line in content.splitlines()])


def _trim_final_newlines(content):
    lines = content.splitlines()
    while lines and lines[-1] == "":
        lines.pop()
    return "".join([line + "\n" for line in lines])


class _CXXFormatter(_Formatter):
    def reformat(self, path):
        with open(path, mode="r", encoding="utf-8") as f:
            content = f.read()
        reformatted_content = _CXXFormatter._reformat_content(content)
        if reformatted_content == content:
            return False
        with open(path, mode="w", encoding="utf-8") as f:
            f.write(reformatted_content)
        return True

    def check(self, path):
        with open(path, mode="r", encoding="utf-8") as f:
            content = f.read()
        return content == _CXXFormatter._reformat_content(content)

    @staticmethod
    def _sort_includes(content):
        lines = content.splitlines()
        result_lines = []
        for is_include, group in groupby(
            lines,
            key=lambda line: bool(_INCLUDE_PATTERN.search(line)),
        ):
            if is_include:
                result_lines.extend(
                    sorted(
                        group,
                        key=lambda line: _INCLUDE_PATTERN.search(line).group(1).lower(),
                    )
                )
            else:
                result_lines.extend(group)
        return "".join([line + "\n" for line in result_lines])

    @staticmethod
    def _reformat_content(content):
        content = _expand_tabs(content)
        content = _trim_trailing_whitespace(content)
        content = _trim_final_newlines(content)
        content = _CXXFormatter._sort_includes(content)
        return content


class _CMakeFormatter(_Formatter):
    def reformat(self, path):
        with open(path, mode="r", encoding="utf-8") as f:
            content = f.read()
        reformatted_content = _CMakeFormatter._reformat_content(content)
        if reformatted_content == content:
            return False
        with open(path, mode="w", encoding="utf-8") as f:
            f.write(reformatted_content)
        return True

    def check(self, path):
        with open(path, mode="r", encoding="utf-8") as f:
            content = f.read()
        return content == _CMakeFormatter._reformat_content(content)

    @staticmethod
    def _sort_sources(content):
        def replace(command):
            if not command.group(1):
                return command.group(0)
            sources = list(_SOURCE_PATTERN.finditer(command.group(1)))
            new_sources = sorted(sources, key=lambda source: source.group(0).lower())
            replaced = []
            i = 0
            for source, new_source in zip(sources, new_sources):
                replaced.append(command.group(1)[i : source.start()])
                replaced.append(new_source.group(0))
                i = source.end()
            replaced.append(command.group(1)[i:])
            start = command.start(1) - command.start()
            end = command.end(1) - command.start()
            return command.group(0)[:start] + "".join(replaced) + command.group(0)[end:]

        return _COMMAND_PATTERN.sub(replace, content)

    @staticmethod
    def _reformat_content(content):
        content = _expand_tabs(content)
        content = _trim_trailing_whitespace(content)
        content = _trim_final_newlines(content)
        content = _CMakeFormatter._sort_sources(content)
        return content


class _BazelFormatter(_Formatter):
    def __init__(self):
        if shutil.which(_BUILDIFIER) is None:
            raise _FormatError(
                "could not find buildifier; install it with `brew install buildifier`"
            )

    def reformat(self, path):
        with open(path, mode="r", encoding="utf-8") as f:
            content = f.read()
        result = subprocess.run([_BUILDIFIER, str(path)])
        if result.returncode != 0:
            raise _FormatError(f"buildifier exited with code {result.returncode}")
        with open(path, mode="r", encoding="utf-8") as f:
            reformatted_content = f.read()
        return reformatted_content != content

    def check(self, path):
        result = subprocess.run([_BUILDIFIER, "-mode=check", str(path)])
        if result.returncode == 0:
            return True
        if result.returncode == 4:
            return False
        raise _FormatError(f"buildifier exited with code {result.returncode}")


def _create_formatter(path):
    if path.suffix in (".cpp", ".h"):
        return _CXXFormatter()
    if path.suffix == ".cmake" or path.name == "CMakeLists.txt":
        return _CMakeFormatter()
    if path.suffix == ".bzl" or path.name in (
        "BUILD",
        "BUILD.bazel",
        "WORKSPACE",
        "WORKSPACE.bazel",
        "MODULE.bazel",
    ):
        return _BazelFormatter()
    raise _FormatError(f"{path}: unsupported language")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input", nargs="+")
    parser.add_argument("--check", action="store_true")

    args = parser.parse_args()

    exit_code = 0

    try:
        for path in args.input:
            path = Path(path)
            formatter = _create_formatter(path)
            if args.check:
                if not formatter.check(path):
                    print(f"{path}: not properly formatted", file=sys.stderr)
                    exit_code = 1
            else:
                if formatter.reformat(path):
                    print(f"{path}: reformatted")
    except _FormatError as e:
        print(f"error: {e}", file=sys.stderr)
        exit_code = 1

    return exit_code


if __name__ == "__main__":
    sys.exit(main())
