#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
from itertools import groupby
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

_INCLUDE_PATTERN = re.compile(r'^\s*#\s*include\s*[<"]\s*([^\s>"]+)\s*[>"]')
_COMMAND_PATTERN = re.compile(
    r"\badd_(?:library|executable)\s*\(\s*[^\s)]+(?:\s+[A-Z_]+)*((?:\s+[^)\s]+)*)\s*\)"
)
_SOURCE_PATTERN = re.compile(r"[^\s)]+")

_BUILDIFIER = os.getenv("BUILDIFIER", "buildifier")


class _FormatError(Exception):
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


def _reformat_cpp_content(content):
    content = _expand_tabs(content)
    content = _trim_trailing_whitespace(content)
    content = _trim_final_newlines(content)
    content = _sort_includes(content)
    return content


def _reformat_cmake_content(content):
    content = _expand_tabs(content)
    content = _trim_trailing_whitespace(content)
    content = _trim_final_newlines(content)
    content = _sort_sources(content)
    return content


def _ensure_buildifier():
    if shutil.which(_BUILDIFIER) is None:
        raise _FormatError(
            "could not find buildifier; install it with `brew install buildifier`"
        )


def _reformat_cpp(path):
    with open(path, mode="r", encoding="utf-8") as f:
        content = f.read()
    reformatted_content = _reformat_cpp_content(content)
    if reformatted_content == content:
        return False
    with open(path, mode="w", encoding="utf-8") as f:
        f.write(reformatted_content)
    return True


def _reformat_cmake(path):
    with open(path, mode="r", encoding="utf-8") as f:
        content = f.read()
    reformatted_content = _reformat_cmake_content(content)
    if reformatted_content == content:
        return False
    with open(path, mode="w", encoding="utf-8") as f:
        f.write(reformatted_content)
    return True


def _reformat_bazel(path):
    _ensure_buildifier()
    with open(path, mode="r", encoding="utf-8") as f:
        content = f.read()
    result = subprocess.run([_BUILDIFIER, str(path)])
    if result.returncode != 0:
        raise _FormatError(f"buildifier exited with code {result.returncode}")
    with open(path, mode="r", encoding="utf-8") as f:
        reformatted_content = f.read()
    return reformatted_content != content


def _check_cpp(path):
    with open(path, mode="r", encoding="utf-8") as f:
        content = f.read()
    return content == _reformat_cpp_content(content)


def _check_cmake(path):
    with open(path, mode="r", encoding="utf-8") as f:
        content = f.read()
    return content == _reformat_cmake_content(content)


def _check_bazel(path):
    _ensure_buildifier()
    result = subprocess.run([_BUILDIFIER, "-mode=check", str(path)])
    if result.returncode == 0:
        return True
    if result.returncode == 4:
        return False
    raise _FormatError(f"buildifier exited with code {result.returncode}")


def _raise_unsupported(path):
    raise _FormatError(f"{path}: unsupported language")


def _get_language(path):
    if path.suffix in (".cpp", ".h"):
        return "cpp"
    if path.suffix == ".cmake":
        return "cmake"
    if path.name == "CMakeLists.txt":
        return "cmake"
    if path.suffix == ".bzl":
        return "bazel"
    if path.name in (
        "BUILD",
        "BUILD.bazel",
        "WORKSPACE",
        "WORKSPACE.bazel",
        "MODULE.bazel",
    ):
        return "bazel"
    return "unsupported"


_REFORMAT = {
    "cpp": _reformat_cpp,
    "cmake": _reformat_cmake,
    "bazel": _reformat_bazel,
    "unsupported": _raise_unsupported,
}

_CHECK = {
    "cpp": _check_cpp,
    "cmake": _check_cmake,
    "bazel": _check_bazel,
    "unsupported": _raise_unsupported,
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input", nargs="+")
    parser.add_argument("--check", action="store_true")

    args = parser.parse_args()

    exit_code = 0

    try:
        for path in args.input:
            path = Path(path)
            if args.check:
                if not _CHECK[_get_language(path)](path):
                    print(f"{path}: not properly formatted", file=sys.stderr)
                    exit_code = 1
            else:
                if _REFORMAT[_get_language(path)](path):
                    print(f"{path}: reformatted")
    except _FormatError as e:
        print(f"error: {e}", file=sys.stderr)
        exit_code = 1

    return exit_code


if __name__ == "__main__":
    sys.exit(main())
