#include "mini-llvm/utils/FileHandle.h"

#include <cstdio>

#ifdef _WIN32
    #include "mini-llvm/utils/Windows.h"
#endif

using namespace mini_llvm;

FileHandle::FileHandle(const char *path, const char *mode) {
    open(path, mode);
}

FileHandle::~FileHandle() {
    close();
}

void FileHandle::open(const char *path, const char *mode) {
#ifdef _WIN32
    handle_ = _wfopen(windows::widen(path).c_str(), windows::widen(mode).c_str());
#else
    handle_ = fopen(path, mode);
#endif
}

void FileHandle::close() {
    if (handle_ == nullptr) return;
    fclose(handle_);
    handle_ = nullptr;
}
