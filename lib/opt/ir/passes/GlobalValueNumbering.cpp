// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/GlobalValueNumbering.h"

#include <cassert>
#include <cstddef>
#include <functional>
#include <ranges>
#include <typeinfo>
#include <unordered_set>

#include "mini-llvm/ir/Attribute/ReadNone.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/FloatingConstant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/BitCast.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/FCmp.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/FloatingToIntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/IntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntegerToFloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntToPtr.h"
#include "mini-llvm/ir/Instruction/PtrToInt.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingOperator.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/HashCombine.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

// Cliff Click. 1995. Global code motion/global value numbering.
// In Proceedings of the ACM SIGPLAN 1995 conference on
// Programming language design and implementation (PLDI '95).
// Association for Computing Machinery, New York, NY, USA, 246-257.
// https://doi.org/10.1145/207110.207154

namespace {

struct ValueNumber {
    const Value *value;
};

bool operator==(const ValueNumber &lhs, const ValueNumber &rhs) {
    if (lhs.value == rhs.value) {
        return true;
    }
    if (typeid(*lhs.value) != typeid(*rhs.value)) {
        return false;
    }
    if (dynamic_cast<const IntegerConstant *>(lhs.value)) {
        auto *lhsValue = static_cast<const IntegerConstant *>(lhs.value),
             *rhsValue = static_cast<const IntegerConstant *>(rhs.value);
        return *lhsValue == *rhsValue;
    }
    if (dynamic_cast<const FloatingConstant *>(lhs.value)) {
        auto *lhsValue = static_cast<const FloatingConstant *>(lhs.value),
             *rhsValue = static_cast<const FloatingConstant *>(rhs.value);
        return *lhsValue == *rhsValue;
    }
    if (dynamic_cast<const BinaryIntegerArithmeticOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const BinaryIntegerArithmeticOperator *>(lhs.value),
             *rhsValue = static_cast<const BinaryIntegerArithmeticOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->lhs()} == ValueNumber{&*rhsValue->lhs()} &&
               ValueNumber{&*lhsValue->rhs()} == ValueNumber{&*rhsValue->rhs()};
    }
    if (dynamic_cast<const ICmp *>(lhs.value)) {
        auto *lhsValue = static_cast<const ICmp *>(lhs.value),
             *rhsValue = static_cast<const ICmp *>(rhs.value);
        return lhsValue->cond() == rhsValue->cond() &&
               ValueNumber{&*lhsValue->lhs()} == ValueNumber{&*rhsValue->lhs()} &&
               ValueNumber{&*lhsValue->rhs()} == ValueNumber{&*rhsValue->rhs()};
    }
    if (dynamic_cast<const BinaryFloatingArithmeticOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const BinaryFloatingArithmeticOperator *>(lhs.value),
             *rhsValue = static_cast<const BinaryFloatingArithmeticOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->lhs()} == ValueNumber{&*rhsValue->lhs()} &&
               ValueNumber{&*lhsValue->rhs()} == ValueNumber{&*rhsValue->rhs()};
    }
    if (dynamic_cast<const UnaryFloatingOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const UnaryFloatingOperator *>(lhs.value),
             *rhsValue = static_cast<const UnaryFloatingOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()};
    }
    if (dynamic_cast<const FCmp *>(lhs.value)) {
        auto *lhsValue = static_cast<const FCmp *>(lhs.value),
             *rhsValue = static_cast<const FCmp *>(rhs.value);
        return lhsValue->cond() == rhsValue->cond() &&
               ValueNumber{&*lhsValue->lhs()} == ValueNumber{&*rhsValue->lhs()} &&
               ValueNumber{&*lhsValue->rhs()} == ValueNumber{&*rhsValue->rhs()};
    }
    if (dynamic_cast<const IntegerCastingOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const IntegerCastingOperator *>(lhs.value),
             *rhsValue = static_cast<const IntegerCastingOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const FloatingCastingOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const FloatingCastingOperator *>(lhs.value),
             *rhsValue = static_cast<const FloatingCastingOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const IntegerToFloatingCastingOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const IntegerToFloatingCastingOperator *>(lhs.value),
             *rhsValue = static_cast<const IntegerToFloatingCastingOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const FloatingToIntegerCastingOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const FloatingToIntegerCastingOperator *>(lhs.value),
             *rhsValue = static_cast<const FloatingToIntegerCastingOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const PtrToInt *>(lhs.value)) {
        auto *lhsValue = static_cast<const PtrToInt *>(lhs.value),
             *rhsValue = static_cast<const PtrToInt *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const IntToPtr *>(lhs.value)) {
        auto *lhsValue = static_cast<const IntToPtr *>(lhs.value),
             *rhsValue = static_cast<const IntToPtr *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const BitCast *>(lhs.value)) {
        auto *lhsValue = static_cast<const BitCast *>(lhs.value),
             *rhsValue = static_cast<const BitCast *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} &&
               *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const Select *>(lhs.value)) {
        auto *lhsValue = static_cast<const Select *>(lhs.value),
             *rhsValue = static_cast<const Select *>(rhs.value);
        return ValueNumber{&*lhsValue->cond()} == ValueNumber{&*rhsValue->cond()} &&
               ValueNumber{&*lhsValue->trueValue()} == ValueNumber{&*rhsValue->trueValue()} &&
               ValueNumber{&*lhsValue->falseValue()} == ValueNumber{&*rhsValue->falseValue()};
    }
    if (dynamic_cast<const Call *>(lhs.value)) {
        auto *lhsValue = static_cast<const Call *>(lhs.value),
             *rhsValue = static_cast<const Call *>(rhs.value);
        if (&*lhsValue->callee() != &*rhsValue->callee()) {
            return false;
        }
        const Function *callee = &*lhsValue->callee();
        if (!callee->attr<ReadNone>()) {
            return false;
        }
        if (lhsValue->arg_size() != rhsValue->arg_size()) {
            return false;
        }
        for (auto [lhsArg, rhsArg] : std::views::zip(args(*lhsValue), args(*rhsValue))) {
            if (ValueNumber{&*lhsArg} != ValueNumber{&*rhsArg}) {
                return false;
            }
        }
        return true;
    }
    return false;
}

} // namespace

template <>
struct std::hash<std::type_info> {
    size_t operator()(const std::type_info &type) const noexcept {
        return type.hash_code();
    }
};

template <>
struct std::hash<Type> {
    size_t operator()(const Type &type) const noexcept {
        assert(dynamic_cast<const IntegerType *>(&type) || dynamic_cast<const FloatingType *>(&type) || type == Ptr());
        return typeid(type).hash_code();
    }
};

template <>
struct std::hash<ValueNumber> {
    size_t operator()(const ValueNumber &number) const noexcept {
        if (auto *value = dynamic_cast<const IntegerConstant *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, value->signExtendedValue());
            return seed;
        }
        if (auto *value = dynamic_cast<const FloatingConstant *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, value->bitPattern());
            return seed;
        }
        if (auto *value = dynamic_cast<const BinaryIntegerArithmeticOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->lhs()});
            hash_combine(seed, ValueNumber{&*value->rhs()});
            return seed;
        }
        if (auto *value = dynamic_cast<const ICmp *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, value->cond());
            hash_combine(seed, ValueNumber{&*value->lhs()});
            hash_combine(seed, ValueNumber{&*value->rhs()});
            return seed;
        }
        if (auto *value = dynamic_cast<const BinaryFloatingArithmeticOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->lhs()});
            hash_combine(seed, ValueNumber{&*value->rhs()});
            return seed;
        }
        if (auto *value = dynamic_cast<const UnaryFloatingOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            return seed;
        }
        if (auto *value = dynamic_cast<const FCmp *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, value->cond());
            hash_combine(seed, ValueNumber{&*value->lhs()});
            hash_combine(seed, ValueNumber{&*value->rhs()});
            return seed;
        }
        if (auto *value = dynamic_cast<const IntegerCastingOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const FloatingCastingOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const IntegerToFloatingCastingOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const FloatingToIntegerCastingOperator *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const PtrToInt *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const IntToPtr *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const BitCast *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->value()});
            hash_combine(seed, *value->type());
            return seed;
        }
        if (auto *value = dynamic_cast<const Select *>(number.value)) {
            size_t seed = 0;
            hash_combine(seed, typeid(*value));
            hash_combine(seed, ValueNumber{&*value->cond()});
            hash_combine(seed, ValueNumber{&*value->trueValue()});
            hash_combine(seed, ValueNumber{&*value->falseValue()});
            return seed;
        }
        if (auto *value = dynamic_cast<const Call *>(number.value)) {
            const Function *callee = &*value->callee();
            if (!callee->attr<ReadNone>()) {
                return reinterpret_cast<size_t>(value);
            }
            size_t seed = 0;
            hash_combine(seed, ValueNumber{callee});
            for (const Use<Value> &arg : args(*value)) {
                hash_combine(seed, ValueNumber{&*arg});
            }
            return seed;
        }
        return reinterpret_cast<size_t>(number.value);
    }
};

namespace {

void dfs(const DTNode *node, std::unordered_set<ValueNumber> &valueNumbers, bool &changed) {
    std::unordered_set<ValueNumber> newValueNumbers;

    for (auto i = node->block->begin(); i != node->block->end();) {
        const Instruction &I = *i++;
        ValueNumber number{&I};
        if (auto j = valueNumbers.find(number); j != valueNumbers.end()) {
            replaceAllUsesWith(I, share(*const_cast<Value *>(j->value)));
            removeFromParent(I);
            changed = true;
        } else {
            valueNumbers.insert(number);
            newValueNumbers.insert(number);
        }
    }

    for (const DTNode *child : node->children) {
        dfs(child, valueNumbers, changed);
    }

    for (const ValueNumber &number : newValueNumbers) {
        valueNumbers.erase(number);
    }
}

} // namespace

bool GlobalValueNumbering::runOnFunction(Function &F) {
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    bool changed = false;

    std::unordered_set<ValueNumber> valueNumbers;

    dfs(domTree.node(F.entry()), valueNumbers, changed);
    assert(valueNumbers.empty());

    assert(F.isWellFormed());
    return changed;
}
