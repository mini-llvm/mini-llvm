// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/TailCallElimination.h"

#include <cassert>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

namespace {

bool isEliminable(const BasicBlock &B) {
    if (B.size() < 2) {
        return false;
    }
    auto *ret = dynamic_cast<const Ret *>(&*std::prev(B.end(), 1));
    if (!ret) {
        return false;
    }
    auto *call = dynamic_cast<const Call *>(&*std::prev(B.end(), 2));
    if (!call) {
        return false;
    }
    if (&*call->callee() != call->parent()->parent()) {
        return false;
    }
    if (*call->type() != Void() && &*ret->value() != call) {
        return false;
    }
    return true;
}

} // namespace

bool TailCallElimination::runOnFunction(Function &F) {
    if (F.functionType()->isVarArgs()) {
        return false;
    }

    std::vector<BasicBlock *> eliminable;
    for (BasicBlock &B : F) {
        if (isEliminable(B)) {
            eliminable.push_back(&B);
        }
    }

    if (eliminable.empty()) {
        return false;
    }

    BasicBlock &oldEntry = F.entry();

    std::vector<std::shared_ptr<Instruction>> allocas;

    for (auto i = oldEntry.begin(); i != oldEntry.end();) {
        Instruction &I = *i++;
        if (dynamic_cast<Alloca *>(&I)) {
            allocas.push_back(share(I));
            removeFromParent(I);
        }
    }

    HashMap<Argument *, Phi *> phis;

    for (Argument &arg : args(F)) {
        Phi &phi = oldEntry.prepend(std::make_shared<Phi>(arg.type()));
        phis.put(&arg, &phi);
    }

    for (Argument &arg : args(F)) {
        replaceAllUsesWith(arg, share(*phis[&arg]));
    }

    BasicBlock &newEntry = F.prepend();

    for (auto &I : allocas) {
        newEntry.append(std::move(I));
    }

    newEntry.append(std::make_shared<Br>(weaken(oldEntry)));

    for (Argument &arg : args(F)) {
        phis[&arg]->addIncoming(weaken(newEntry), share(arg));
    }

    for (BasicBlock *B : eliminable) {
        auto *ret = static_cast<Ret *>(&*std::prev(B->end(), 1));
        auto *call = static_cast<Call *>(&*std::prev(B->end(), 2));

        for (auto [functionArg, callArg] : std::views::zip(args(F), args(*call))) {
            phis[&functionArg]->addIncoming(weaken(*B), share(*callArg));
        }

        B->append(std::make_shared<Br>(weaken(oldEntry)));

        removeFromParent(*ret);
        removeFromParent(*call);
    }

    assert(F.isWellFormed());
    return true;
}
