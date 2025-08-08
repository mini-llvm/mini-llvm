// SPDX-License-Identifier: MIT

#include "mini-llvm/utils/ScopedFileHandle.h"

#include <cstdio>

#include "mini-llvm/utils/Path.h"
#include "mini-llvm/utils/SystemString.h"

using namespace mini_llvm;

ScopedFileHandle::ScopedFileHandle(const Path &path, const SystemString &mode) {
    open(path, mode);
}

ScopedFileHandle::~ScopedFileHandle() {
    close();
}

void ScopedFileHandle::open(const Path &path, const SystemString &mode) {
#ifdef _WIN32
    handle_ = _wfopen(path.base().c_str(), mode.c_str());
#else
    handle_ = fopen(path.base().c_str(), mode.c_str());
#endif
}

void ScopedFileHandle::close() {
    if (handle_ == nullptr) return;
    fclose(handle_);
    handle_ = nullptr;
}

FILE *ScopedFileHandle::release() {
    FILE *handle = handle_;
    handle_ = nullptr;
    return handle;
}
