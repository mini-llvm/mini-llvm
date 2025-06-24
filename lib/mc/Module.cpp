#include "mini-llvm/mc/Module.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

GlobalValue &Module::add(const_iterator pos, GlobalValue G) {
    return *globalValues_.insert(pos, std::move(G));
}

void Module::remove(const_iterator pos) {
    globalValues_.erase(pos);
}

void Module::clear() {
    globalValues_.clear();
}

std::string Module::format() const {
    std::vector<const GlobalValue *> data, rodata, bss, text;
    for (const GlobalValue &G : *this) {
        switch (G.section()) {
            case Section::kData: data.push_back(&G); break;
            case Section::kROData: rodata.push_back(&G); break;
            case Section::kBSS: bss.push_back(&G); break;
            case Section::kText: text.push_back(&G); break;
            default: abort();
        }
    }
    StringJoiner formatted("\n");
    if (!data.empty()) {
        formatted.add("  .data");
        for (const GlobalValue *G : data) {
            formatted.add("{}", *G);
        }
    }
    if (!rodata.empty()) {
        formatted.add("  .section .rodata");
        for (const GlobalValue *G : rodata) {
            formatted.add("{}", *G);
        }
    }
    if (!bss.empty()) {
        formatted.add("  .bss");
        for (const GlobalValue *G : bss) {
            formatted.add("{}", *G);
        }
    }
    if (!text.empty()) {
        formatted.add("  .text");
        for (const GlobalValue *G : text) {
            formatted.add("{}", *G);
        }
    }
    return formatted.toString();
}
