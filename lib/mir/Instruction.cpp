#include "mini-llvm/mir/Instruction.h"

#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/RegisterOperand.h"

using namespace mini_llvm::mir;

std::unordered_set<RegisterOperand *> Instruction::regOps() {
    std::unordered_set<RegisterOperand *> regOps;
    for (const RegisterOperand *op : std::as_const(*this).regOps()) {
        regOps.insert(const_cast<RegisterOperand *>(op));
    }
    return regOps;
}

std::unordered_set<RegisterOperand *> Instruction::srcs() {
    std::unordered_set<RegisterOperand *> srcs;
    for (const RegisterOperand *op : std::as_const(*this).srcs()) {
        srcs.insert(const_cast<RegisterOperand *>(op));
    }
    return srcs;
}

std::unordered_set<RegisterOperand *> Instruction::dsts() {
    std::unordered_set<RegisterOperand *> dsts;
    for (const RegisterOperand *op : std::as_const(*this).dsts()) {
        dsts.insert(const_cast<RegisterOperand *>(op));
    }
    return dsts;
}

std::unordered_set<Register *> mini_llvm::mir::use(const Instruction &I) {
    std::unordered_set<Register *> use;
    for (const RegisterOperand *op : I.srcs()) {
        use.insert(&**op);
    }
    for (PhysicalRegister *reg : I.implicitSrcs()) {
        use.insert(reg);
    }
    return use;
}

std::unordered_set<Register *> mini_llvm::mir::def(const Instruction &I) {
    std::unordered_set<Register *> def;
    for (const RegisterOperand *op : I.dsts()) {
        def.insert(&**op);
    }
    for (PhysicalRegister *reg : I.implicitDsts()) {
        def.insert(reg);
    }
    return def;
}
