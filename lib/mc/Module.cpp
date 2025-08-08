// SPDX-License-Identifier: MIT

#include "mini-llvm/mc/Module.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/utils/HashMap.h"
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
    HashMap<std::string, std::vector<const GlobalValue *>> sections;
    for (const GlobalValue &G : *this) {
        sections.put(G.section(), {});
    }
    for (const GlobalValue &G : *this) {
        sections[G.section()].push_back(&G);
    }
    StringJoiner formatted("\n");
    for (const auto &[section, globalValues] : sections) {
        if (section == ".text" || section == ".data" || section == ".bss") {
            formatted.add("  {}", section);
        } else {
            formatted.add("  .section {}", section);
        }
        for (const GlobalValue *G : globalValues) {
            formatted.add("{}", *G);
        }
    }
    return formatted.toString();
}
