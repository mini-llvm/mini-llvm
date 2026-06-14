#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
import os
import shutil
import subprocess
import sys
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed

_CLANG_TIDY = os.getenv("CLANG_TIDY", "clang-tidy")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p")
    parser.add_argument("--quiet", action="store_true")
    parser.add_argument("-j", type=int, default=os.cpu_count() or 1)
    parser.add_argument("paths", nargs="+")
    args = parser.parse_args()

    if shutil.which(_CLANG_TIDY) is None:
        print(
            "error: could not find clang-tidy; install it with `brew install llvm`",
            file=sys.stderr,
        )
        return 1

    base_args = [_CLANG_TIDY]
    if args.p is not None:
        base_args += ["-p", args.p]
    if args.quiet:
        base_args.append("--quiet")

    total = len(args.paths)
    finished = 0
    lock = threading.Lock()
    stop = threading.Event()

    def run(path):
        nonlocal finished
        result = subprocess.run(
            base_args + [path],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
        )
        with lock:
            if stop.is_set():
                return True
            finished += 1
            print(f"[{finished}/{total}] {path}", flush=True)
            print(result.stdout, end="", file=sys.stderr, flush=True)
        return result.returncode == 0

    exit_code = 0
    executor = ThreadPoolExecutor(max_workers=max(args.j, 1))
    futures = [executor.submit(run, path) for path in args.paths]
    try:
        for future in as_completed(futures):
            if not future.result():
                exit_code = 1
    except KeyboardInterrupt:
        stop.set()
        exit_code = 130
    finally:
        executor.shutdown(wait=False, cancel_futures=True)

    return exit_code


if __name__ == "__main__":
    sys.exit(main())
