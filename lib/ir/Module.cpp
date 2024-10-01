#include "mini-llvm/ir/Module.h"

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

GlobalVar &Module::GlobalVars::add(Module::GlobalVars::const_iterator pos, std::shared_ptr<GlobalVar> G) {
    return **globalVars_.insert(pos.base(), std::move(G));
}

void Module::GlobalVars::remove(Module::GlobalVars::const_iterator pos) {
    globalVars_.erase(pos.base());
}

void Module::GlobalVars::clear() {
    globalVars_.clear();
}

Function &Module::Functions::add(Module::Functions::const_iterator pos, std::shared_ptr<Function> F) {
    return **functions_.insert(pos.base(), std::move(F));
}

void Module::Functions::remove(Module::Functions::const_iterator pos) {
    functions_.erase(pos.base());
}

void Module::Functions::clear() {
    functions_.clear();
}

std::string Module::format() const {
    StringJoiner formatted("\n\n");
    for (const GlobalVar &G : globalVars) {
        formatted.addFormat("{}", G);
    }
    for (const Function &F : functions) {
        formatted.addFormat("{}", F);
    }
    return formatted.toString();
}
