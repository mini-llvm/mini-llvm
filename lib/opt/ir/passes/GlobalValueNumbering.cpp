#include "mini-llvm/opt/ir/passes/GlobalValueNumbering.h"

#include <cassert>
#include <functional>
#include <typeinfo>
#include <unordered_set>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/FloatingConstant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingRelationalOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/BitCast.h"
#include "mini-llvm/ir/Instruction/FCmp.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/IntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntToPtr.h"
#include "mini-llvm/ir/Instruction/PtrToInt.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingOperator.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/Hash.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

struct ValueNumber {
    const Value *value;
};

bool operator==(const ValueNumber &lhs, const ValueNumber &rhs) {
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
    if (dynamic_cast<const BinaryFloatingRelationalOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const BinaryFloatingRelationalOperator *>(lhs.value),
             *rhsValue = static_cast<const BinaryFloatingRelationalOperator *>(rhs.value);
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
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} && *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const FloatingCastingOperator *>(lhs.value)) {
        auto *lhsValue = static_cast<const FloatingCastingOperator *>(lhs.value),
             *rhsValue = static_cast<const FloatingCastingOperator *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} && *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const PtrToInt *>(lhs.value)) {
        auto *lhsValue = static_cast<const PtrToInt *>(lhs.value),
             *rhsValue = static_cast<const PtrToInt *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} && *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const IntToPtr *>(lhs.value)) {
        auto *lhsValue = static_cast<const IntToPtr *>(lhs.value),
             *rhsValue = static_cast<const IntToPtr *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} && *lhsValue->type() == *rhsValue->type();
    }
    if (dynamic_cast<const BitCast *>(lhs.value)) {
        auto *lhsValue = static_cast<const BitCast *>(lhs.value),
             *rhsValue = static_cast<const BitCast *>(rhs.value);
        return ValueNumber{&*lhsValue->value()} == ValueNumber{&*rhsValue->value()} && *lhsValue->type() == *rhsValue->type();
    }
    return lhs.value == rhs.value;
}

} // namespace

template <>
struct std::hash<std::type_info> {
    constexpr size_t operator()(const std::type_info &type) const noexcept {
        return type.hash_code();
    }
};

template <>
struct std::hash<Type> {
    constexpr size_t operator()(const Type &type) const noexcept {
        assert(dynamic_cast<const IntegerType *>(&type) || dynamic_cast<const FloatingType *>(&type));
        return typeid(type).hash_code();
    }
};

template <>
struct std::hash<ValueNumber> {
    constexpr size_t operator()(const ValueNumber &number) const noexcept {
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
        if (auto *value = dynamic_cast<const BinaryFloatingRelationalOperator *>(number.value)) {
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
        return reinterpret_cast<size_t>(number.value);
    }
};

namespace {

void dfs(const DominatorTreeNode *node, std::unordered_set<ValueNumber> &valueNumbers, bool &changed) {
    std::unordered_set<ValueNumber> newValueNumbers;

    for (const Instruction &I : *node->block) {
        ValueNumber number{&I};
        if (auto i = valueNumbers.find(number); i != valueNumbers.end()) {
            changed |= replaceAllUsesWith(I, share(*const_cast<Value *>(i->value)));
        } else {
            valueNumbers.insert(number);
            newValueNumbers.insert(number);
        }
    }

    for (const DominatorTreeNode *child : node->children) {
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

    return changed;
}
