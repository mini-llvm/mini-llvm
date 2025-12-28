// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/codegen/RISCVMIRGen.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/FloatingConstant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/Constant/PointerConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/And.h"
#include "mini-llvm/ir/Instruction/ASHR.h"
#include "mini-llvm/ir/Instruction/BitCast.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/CondBr.h"
#include "mini-llvm/ir/Instruction/FAdd.h"
#include "mini-llvm/ir/Instruction/FCmp.h"
#include "mini-llvm/ir/Instruction/FDiv.h"
#include "mini-llvm/ir/Instruction/FMul.h"
#include "mini-llvm/ir/Instruction/FNeg.h"
#include "mini-llvm/ir/Instruction/FPExt.h"
#include "mini-llvm/ir/Instruction/FPToSI.h"
#include "mini-llvm/ir/Instruction/FPToUI.h"
#include "mini-llvm/ir/Instruction/FPTrunc.h"
#include "mini-llvm/ir/Instruction/FSub.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/IndirectCall.h"
#include "mini-llvm/ir/Instruction/IntToPtr.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/LSHR.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/Or.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/PtrToInt.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Instruction/SDiv.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Instruction/SHL.h"
#include "mini-llvm/ir/Instruction/SIToFP.h"
#include "mini-llvm/ir/Instruction/SRem.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Instruction/Sub.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Instruction/UDiv.h"
#include "mini-llvm/ir/Instruction/UIToFP.h"
#include "mini-llvm/ir/Instruction/URem.h"
#include "mini-llvm/ir/Instruction/Xor.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/Type/IntegerOrPointerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/Constant/I16ArrayConstant.h"
#include "mini-llvm/mir/Constant/I16Constant.h"
#include "mini-llvm/mir/Constant/I32ArrayConstant.h"
#include "mini-llvm/mir/Constant/I32Constant.h"
#include "mini-llvm/mir/Constant/I64ArrayConstant.h"
#include "mini-llvm/mir/Constant/I64Constant.h"
#include "mini-llvm/mir/Constant/I8ArrayConstant.h"
#include "mini-llvm/mir/Constant/I8Constant.h"
#include "mini-llvm/mir/Constant/PtrArrayConstant.h"
#include "mini-llvm/mir/Constant/PtrConstant.h"
#include "mini-llvm/mir/Constant/ZeroConstant.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/AddI.h"
#include "mini-llvm/mir/Instruction/And.h"
#include "mini-llvm/mir/Instruction/AndI.h"
#include "mini-llvm/mir/Instruction/Br.h"
#include "mini-llvm/mir/Instruction/CmpBr.h"
#include "mini-llvm/mir/Instruction/CmpSet.h"
#include "mini-llvm/mir/Instruction/CmpZBr.h"
#include "mini-llvm/mir/Instruction/CmpZSet.h"
#include "mini-llvm/mir/Instruction/FAdd.h"
#include "mini-llvm/mir/Instruction/FCmpSet.h"
#include "mini-llvm/mir/Instruction/FCvt.h"
#include "mini-llvm/mir/Instruction/FCvtFS.h"
#include "mini-llvm/mir/Instruction/FCvtFU.h"
#include "mini-llvm/mir/Instruction/FCvtSF.h"
#include "mini-llvm/mir/Instruction/FCvtUF.h"
#include "mini-llvm/mir/Instruction/FDiv.h"
#include "mini-llvm/mir/Instruction/FLoad.h"
#include "mini-llvm/mir/Instruction/FMov.h"
#include "mini-llvm/mir/Instruction/FMovFI.h"
#include "mini-llvm/mir/Instruction/FMovIF.h"
#include "mini-llvm/mir/Instruction/FMul.h"
#include "mini-llvm/mir/Instruction/FNeg.h"
#include "mini-llvm/mir/Instruction/FStore.h"
#include "mini-llvm/mir/Instruction/FSub.h"
#include "mini-llvm/mir/Instruction/LA.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Load.h"
#include "mini-llvm/mir/Instruction/Marker.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Instruction/Mul.h"
#include "mini-llvm/mir/Instruction/Neg.h"
#include "mini-llvm/mir/Instruction/Or.h"
#include "mini-llvm/mir/Instruction/SDiv.h"
#include "mini-llvm/mir/Instruction/SExt.h"
#include "mini-llvm/mir/Instruction/SHL.h"
#include "mini-llvm/mir/Instruction/SHLI.h"
#include "mini-llvm/mir/Instruction/SHRA.h"
#include "mini-llvm/mir/Instruction/SHRAI.h"
#include "mini-llvm/mir/Instruction/SHRL.h"
#include "mini-llvm/mir/Instruction/SHRLI.h"
#include "mini-llvm/mir/Instruction/SRem.h"
#include "mini-llvm/mir/Instruction/Store.h"
#include "mini-llvm/mir/Instruction/Sub.h"
#include "mini-llvm/mir/Instruction/UDiv.h"
#include "mini-llvm/mir/Instruction/URem.h"
#include "mini-llvm/mir/Instruction/Xor.h"
#include "mini-llvm/mir/Instruction/XorI.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/StackOffsetImmediate.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVCall.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVJALR.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVRet.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

void flatten(const ir::Constant &C, std::vector<const ir::Constant *> &flattened) {
    if (dynamic_cast<const ir::ArrayConstant *>(&C)) {
        for (const ir::Use<ir::Constant> &element : elements(*static_cast<const ir::ArrayConstant *>(&C))) {
            flatten(*element, flattened);
        }
    } else {
        flattened.push_back(&C);
    }
}

std::pair<GlobalValue *, int64_t> emitPtrConstantValue(
    const ir::PointerConstant &C,
    const HashMap<const ir::GlobalVar *, GlobalVar *> &globalVarMap,
    const HashMap<const ir::Function *, Function *> &functionMap
) {
    if (auto *G = dynamic_cast<const ir::GlobalVar *>(&C)) {
        return {globalVarMap[G], 0};
    }
    if (auto *F = dynamic_cast<const ir::Function *>(&C)) {
        return {functionMap[F], 0};
    }
    if (dynamic_cast<const ir::NullPtrConstant *>(&C)) {
        return {nullptr, 0};
    }
    abort();
}

std::unique_ptr<Constant> emitConstant(
    const ir::Constant &C,
    const HashMap<const ir::GlobalVar *, GlobalVar *> &globalVarMap,
    const HashMap<const ir::Function *, Function *> &functionMap
) {
    if (auto *pointerConstant = dynamic_cast<const ir::PointerConstant *>(&C)) {
        return std::make_unique<PtrConstant>(8, emitPtrConstantValue(*pointerConstant, globalVarMap, functionMap));
    }
    if (C == *C.type()->zeroValue()) {
        return std::make_unique<ZeroConstant>(C.type()->size(8));
    }
    if (auto *i1Constant = dynamic_cast<const ir::I1Constant *>(&C)) {
        return std::make_unique<I8Constant>(static_cast<int8_t>(i1Constant->value()));
    }
    if (auto *i8Constant = dynamic_cast<const ir::I8Constant *>(&C)) {
        return std::make_unique<I8Constant>(i8Constant->value());
    }
    if (auto *i16Constant = dynamic_cast<const ir::I16Constant *>(&C)) {
        return std::make_unique<I16Constant>(i16Constant->value());
    }
    if (auto *i32Constant = dynamic_cast<const ir::I32Constant *>(&C)) {
        return std::make_unique<I32Constant>(i32Constant->value());
    }
    if (auto *i64Constant = dynamic_cast<const ir::I64Constant *>(&C)) {
        return std::make_unique<I64Constant>(i64Constant->value());
    }
    if (auto *floatConstant = dynamic_cast<const ir::FloatConstant *>(&C)) {
        return std::make_unique<I32Constant>(std::bit_cast<int32_t>(floatConstant->value()));
    }
    if (auto *doubleConstant = dynamic_cast<const ir::DoubleConstant *>(&C)) {
        return std::make_unique<I64Constant>(std::bit_cast<int64_t>(doubleConstant->value()));
    }
    if (auto *arrayConstant = dynamic_cast<const ir::ArrayConstant *>(&C)) {
        std::vector<const ir::Constant *> flattened;
        flatten(*arrayConstant, flattened);

        std::unique_ptr<ir::Type> type = arrayConstant->type();
        while (dynamic_cast<const ir::ArrayType *>(&*type)) {
            type = static_cast<const ir::ArrayType *>(&*type)->elementType();
        }

        if (*type == ir::I1()) {
            std::vector<int8_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(static_cast<int8_t>(static_cast<const ir::I1Constant *>(element)->value()));
            }
            return std::make_unique<I8ArrayConstant>(std::move(elements));
        }
        if (*type == ir::I8()) {
            std::vector<int8_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(static_cast<const ir::I8Constant *>(element)->value());
            }
            return std::make_unique<I8ArrayConstant>(std::move(elements));
        }
        if (*type == ir::I16()) {
            std::vector<int16_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(static_cast<const ir::I16Constant *>(element)->value());
            }
            return std::make_unique<I16ArrayConstant>(std::move(elements));
        }
        if (*type == ir::I32()) {
            std::vector<int32_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(static_cast<const ir::I32Constant *>(element)->value());
            }
            return std::make_unique<I32ArrayConstant>(std::move(elements));
        }
        if (*type == ir::I64()) {
            std::vector<int64_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(static_cast<const ir::I64Constant *>(element)->value());
            }
            return std::make_unique<I64ArrayConstant>(std::move(elements));
        }
        if (*type == ir::Float()) {
            std::vector<int32_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(std::bit_cast<int32_t>(static_cast<const ir::FloatConstant *>(element)->value()));
            }
            return std::make_unique<I32ArrayConstant>(std::move(elements));
        }
        if (*type == ir::Double()) {
            std::vector<int64_t> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(std::bit_cast<int64_t>(static_cast<const ir::DoubleConstant *>(element)->value()));
            }
            return std::make_unique<I64ArrayConstant>(std::move(elements));
        }
        if (*type == ir::Ptr()) {
            std::vector<std::pair<GlobalValue *, int64_t>> elements;
            for (const ir::Constant *element : flattened) {
                elements.push_back(emitPtrConstantValue(*static_cast<const ir::PointerConstant *>(element), globalVarMap, functionMap));
            }
            return std::make_unique<PtrArrayConstant>(8, std::move(elements));
        }
        abort();
    }
    abort();
}

class InstructionVisitorImpl final : public ir::InstructionVisitor {
public:
    InstructionVisitorImpl(Function &function,
                           const HashMap<const ir::GlobalVar *, GlobalVar *> &globalVarMap,
                           const HashMap<const ir::Function *, Function *> &functionMap,
                           const HashMap<const ir::BasicBlock *, BasicBlock *> &blockMap,
                           const HashMap<const ir::Alloca *, StackSlot *> &memoryMap,
                           const HashMap<const ir::Value *, std::shared_ptr<Register>> &valueMap,
                           BasicBlock *epilogueBlock)
        : function_(function),
          globalVarMap_(globalVarMap),
          functionMap_(functionMap),
          blockMap_(blockMap),
          memoryMap_(memoryMap),
          valueMap_(valueMap),
          epilogueBlock_(epilogueBlock) {}

    BasicBlockBuilder &builder() {
        return builder_;
    }

    const BasicBlockBuilder &builder() const {
        return builder_;
    }

    void visitAdd(const ir::Add &I) override {
        visitBinaryIntegerArithmeticOperator<ir::Add, Add>(I);
    }

    void visitSub(const ir::Sub &I) override {
        visitBinaryIntegerArithmeticOperator<ir::Sub, Sub>(I);
    }

    void visitMul(const ir::Mul &I) override {
        visitBinaryIntegerArithmeticOperator<ir::Mul, Mul>(I);
    }

    void visitSDiv(const ir::SDiv &I) override {
        visitBinaryIntegerArithmeticOperator<ir::SDiv, SDiv>(I);
    }

    void visitUDiv(const ir::UDiv &I) override {
        visitBinaryIntegerArithmeticOperator<ir::UDiv, UDiv>(I);
    }

    void visitSRem(const ir::SRem &I) override {
        visitBinaryIntegerArithmeticOperator<ir::SRem, SRem>(I);
    }

    void visitURem(const ir::URem &I) override {
        visitBinaryIntegerArithmeticOperator<ir::URem, URem>(I);
    }

    void visitAnd(const ir::And &I) override {
        visitBinaryIntegerBitwiseOperator<ir::And, And>(I);
    }

    void visitOr(const ir::Or &I) override {
        visitBinaryIntegerBitwiseOperator<ir::Or, Or>(I);
    }

    void visitXor(const ir::Xor &I) override {
        visitBinaryIntegerBitwiseOperator<ir::Xor, Xor>(I);
    }

    void visitSHL(const ir::SHL &I) override {
        visitBinaryIntegerArithmeticOperator<ir::SHL, SHL>(I);
    }

    void visitLSHR(const ir::LSHR &I) override {
        visitBinaryIntegerArithmeticOperator<ir::LSHR, SHRL>(I);
    }

    void visitASHR(const ir::ASHR &I) override {
        visitBinaryIntegerArithmeticOperator<ir::ASHR, SHRA>(I);
    }

    void visitTrunc(const ir::Trunc &I) override {
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        int dstBitWidth = I.type()->bitSize(8);
#ifndef NDEBUG
        int srcBitWidth = I.value()->type()->bitSize(8);
        assert(dstBitWidth < srcBitWidth);
#endif
        if (dstBitWidth == 32) {
            builder_.add(std::make_unique<SExt>(8, 4, std::move(dst), std::move(src)));
        } else {
            builder_.add(std::make_unique<Mov>(8, dst, src));
            builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - dstBitWidth)));
            builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - dstBitWidth)));
        }
    }

    void visitSExt(const ir::SExt &I) override {
        if (auto *icmp = dynamic_cast<const ir::ICmp *>(&*I.value())) {
            std::shared_ptr<Register> dst = valueMap_[&I],
                                      src1 = getRegister(*icmp->lhs()),
                                      src2 = getRegister(*icmp->rhs());
            switch (icmp->cond()) {
                case ir::ICmp::Condition::kEQ:
                case ir::ICmp::Condition::kNE: {
                    builder_.add(std::make_unique<Xor>(8, dst, src1, src2));

                    Condition cond;
                    switch (icmp->cond()) {
                        case ir::ICmp::Condition::kEQ: cond = Condition::kEQZ; break;
                        case ir::ICmp::Condition::kNE: cond = Condition::kNEZ; break;
                        default: abort();
                    }
                    builder_.add(std::make_unique<CmpZSet>(8, 8, cond, dst, dst));
                    builder_.add(std::make_unique<Neg>(8, dst, dst));

                    break;
                }

                case ir::ICmp::Condition::kSLT:
                case ir::ICmp::Condition::kSGT:
                case ir::ICmp::Condition::kSLE:
                case ir::ICmp::Condition::kSGE:
                case ir::ICmp::Condition::kULT:
                case ir::ICmp::Condition::kUGT:
                case ir::ICmp::Condition::kULE:
                case ir::ICmp::Condition::kUGE: {
                    Condition cond;
                    bool negate;
                    switch (icmp->cond()) {
                        case ir::ICmp::Condition::kSLT: cond = Condition::kSLT; negate = false; break;
                        case ir::ICmp::Condition::kSGT: cond = Condition::kSGT; negate = false; break;
                        case ir::ICmp::Condition::kSLE: cond = Condition::kSGT; negate = true; break;
                        case ir::ICmp::Condition::kSGE: cond = Condition::kSLT; negate = true; break;
                        case ir::ICmp::Condition::kULT: cond = Condition::kULT; negate = false; break;
                        case ir::ICmp::Condition::kUGT: cond = Condition::kUGT; negate = false; break;
                        case ir::ICmp::Condition::kULE: cond = Condition::kUGT; negate = true; break;
                        case ir::ICmp::Condition::kUGE: cond = Condition::kULT; negate = true; break;
                        default: abort();
                    }

                    builder_.add(std::make_unique<CmpSet>(8, 8, cond, dst, src1, src2));
                    builder_.add(std::make_unique<Neg>(8, dst, dst));

                    if (negate) {
                        builder_.add(std::make_unique<XorI>(8, dst, dst, std::make_unique<IntegerImmediate>(-1)));
                    }

                    break;
                }
            }
        } else {
            std::shared_ptr<Register> dst = valueMap_[&I],
                                      src = getRegister(*I.value());
            if (*I.value()->type() == ir::I1()) {
                builder_.add(std::make_unique<Neg>(8, dst, src));
            } else {
                int srcBitWidth = I.value()->type()->bitSize(8);
#ifndef NDEBUG
                int dstBitWidth = I.type()->bitSize(8);
                assert(dstBitWidth > srcBitWidth);
#endif
                if (srcBitWidth == 32) {
                    builder_.add(std::make_unique<SExt>(8, 4, std::move(dst), std::move(src)));
                } else {
                    builder_.add(std::make_unique<Mov>(8, dst, src));
                    builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcBitWidth)));
                    builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcBitWidth)));
                }
            }
        }
    }

    void visitZExt(const ir::ZExt &I) override {
        if (auto *icmp = dynamic_cast<const ir::ICmp *>(&*I.value())) {
            std::shared_ptr<Register> dst = valueMap_[&I],
                                      src1 = getRegister(*icmp->lhs()),
                                      src2 = getRegister(*icmp->rhs());
            switch (icmp->cond()) {
                case ir::ICmp::Condition::kEQ:
                case ir::ICmp::Condition::kNE: {
                    builder_.add(std::make_unique<Xor>(8, dst, src1, src2));

                    Condition cond;
                    switch (icmp->cond()) {
                        case ir::ICmp::Condition::kEQ: cond = Condition::kEQZ; break;
                        case ir::ICmp::Condition::kNE: cond = Condition::kNEZ; break;
                        default: abort();
                    }
                    builder_.add(std::make_unique<CmpZSet>(8, 8, cond, dst, dst));

                    break;
                }

                case ir::ICmp::Condition::kSLT:
                case ir::ICmp::Condition::kSGT:
                case ir::ICmp::Condition::kSLE:
                case ir::ICmp::Condition::kSGE:
                case ir::ICmp::Condition::kULT:
                case ir::ICmp::Condition::kUGT:
                case ir::ICmp::Condition::kULE:
                case ir::ICmp::Condition::kUGE: {
                    Condition cond;
                    bool negate;
                    switch (icmp->cond()) {
                        case ir::ICmp::Condition::kSLT: cond = Condition::kSLT; negate = false; break;
                        case ir::ICmp::Condition::kSGT: cond = Condition::kSGT; negate = false; break;
                        case ir::ICmp::Condition::kSLE: cond = Condition::kSGT; negate = true; break;
                        case ir::ICmp::Condition::kSGE: cond = Condition::kSLT; negate = true; break;
                        case ir::ICmp::Condition::kULT: cond = Condition::kULT; negate = false; break;
                        case ir::ICmp::Condition::kUGT: cond = Condition::kUGT; negate = false; break;
                        case ir::ICmp::Condition::kULE: cond = Condition::kUGT; negate = true; break;
                        case ir::ICmp::Condition::kUGE: cond = Condition::kULT; negate = true; break;
                        default: abort();
                    }

                    builder_.add(std::make_unique<CmpSet>(8, 8, cond, dst, src1, src2));

                    if (negate) {
                        builder_.add(std::make_unique<XorI>(8, dst, dst, std::make_unique<IntegerImmediate>(1)));
                    }

                    break;
                }
            }
        } else {
            std::shared_ptr<Register> dst = valueMap_[&I],
                                      src = getRegister(*I.value());
            if (*I.value()->type() == ir::I1()) {
                builder_.add(std::make_unique<AndI>(8, dst, src, std::make_unique<IntegerImmediate>(1)));
            } else {
                int srcBitWidth = I.value()->type()->bitSize(8);
#ifndef NDEBUG
                int dstBitWidth = I.type()->bitSize(8);
                assert(dstBitWidth > srcBitWidth);
#endif
                if (srcBitWidth == 8) {
                    builder_.add(std::make_unique<AndI>(8, dst, src, std::make_unique<IntegerImmediate>(0xff)));
                } else {
                    builder_.add(std::make_unique<Mov>(8, dst, src));
                    builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcBitWidth)));
                    builder_.add(std::make_unique<SHRLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcBitWidth)));
                }
            }
        }
    }

    void visitICmp(const ir::ICmp &I) override {
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src1 = getRegister(*I.lhs()),
                                  src2 = getRegister(*I.rhs());

        switch (I.cond()) {
            case ir::ICmp::Condition::kEQ:
            case ir::ICmp::Condition::kNE: {
                builder_.add(std::make_unique<Xor>(8, dst, src1, src2));

                Condition cond;
                switch (I.cond()) {
                    case ir::ICmp::Condition::kEQ: cond = Condition::kEQZ; break;
                    case ir::ICmp::Condition::kNE: cond = Condition::kNEZ; break;
                    default: abort();
                }
                builder_.add(std::make_unique<CmpZSet>(8, 8, cond, dst, dst));
                builder_.add(std::make_unique<Neg>(8, dst, dst));

                break;
            }

            case ir::ICmp::Condition::kSLT:
            case ir::ICmp::Condition::kSGT:
            case ir::ICmp::Condition::kSLE:
            case ir::ICmp::Condition::kSGE:
            case ir::ICmp::Condition::kULT:
            case ir::ICmp::Condition::kUGT:
            case ir::ICmp::Condition::kULE:
            case ir::ICmp::Condition::kUGE: {
                Condition cond;
                bool negate;
                switch (I.cond()) {
                    case ir::ICmp::Condition::kSLT: cond = Condition::kSLT; negate = false; break;
                    case ir::ICmp::Condition::kSGT: cond = Condition::kSGT; negate = false; break;
                    case ir::ICmp::Condition::kSLE: cond = Condition::kSGT; negate = true; break;
                    case ir::ICmp::Condition::kSGE: cond = Condition::kSLT; negate = true; break;
                    case ir::ICmp::Condition::kULT: cond = Condition::kULT; negate = false; break;
                    case ir::ICmp::Condition::kUGT: cond = Condition::kUGT; negate = false; break;
                    case ir::ICmp::Condition::kULE: cond = Condition::kUGT; negate = true; break;
                    case ir::ICmp::Condition::kUGE: cond = Condition::kULT; negate = true; break;
                    default: abort();
                }

                builder_.add(std::make_unique<CmpSet>(8, 8, cond, dst, src1, src2));
                builder_.add(std::make_unique<Neg>(8, dst, dst));

                if (negate) {
                    builder_.add(std::make_unique<XorI>(8, dst, dst, std::make_unique<IntegerImmediate>(-1)));
                }

                break;
            }

            default: abort();
        }
    }

    void visitFNeg(const ir::FNeg &I) override {
        visitUnaryFloatingArithmeticOperator<ir::FNeg, FNeg>(I);
    }

    void visitFAdd(const ir::FAdd &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FAdd, FAdd>(I);
    }

    void visitFSub(const ir::FSub &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FSub, FSub>(I);
    }

    void visitFMul(const ir::FMul &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FMul, FMul>(I);
    }

    void visitFDiv(const ir::FDiv &I) override {
        visitBinaryFloatingArithmeticOperator<ir::FDiv, FDiv>(I);
    }

    void visitFPTrunc(const ir::FPTrunc &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision(),
                  srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<FCvt>(dstPrecision, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitFPExt(const ir::FPExt &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision(),
                  srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<FCvt>(dstPrecision, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitSIToFP(const ir::SIToFP &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
        int srcWidth = I.value()->type()->size();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<FCvtFS>(dstPrecision, srcWidth, std::move(dst), std::move(src)));
    }

    void visitUIToFP(const ir::UIToFP &I) override {
        Precision dstPrecision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
        int srcWidth = I.value()->type()->size();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<FCvtFU>(dstPrecision, srcWidth, std::move(dst), std::move(src)));
    }

    void visitFPToSI(const ir::FPToSI &I) override {
        int dstWidth = I.type()->size();
        Precision srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<FCvtSF>(dstWidth, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitFPToUI(const ir::FPToUI &I) override {
        int dstWidth = I.type()->size();
        Precision srcPrecision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<FCvtUF>(dstWidth, srcPrecision, std::move(dst), std::move(src)));
    }

    void visitFCmp(const ir::FCmp &I) override {
        Precision precision = static_cast<const ir::FloatingType *>(&*I.lhs()->type())->precision();
        Condition cond;
        bool negate;
        switch (I.cond()) {
            case ir::FCmp::Condition::kOEQ: cond = Condition::kOEQ; negate = false; break;
            case ir::FCmp::Condition::kONE: cond = Condition::kOEQ; negate = true; break;
            case ir::FCmp::Condition::kOLT: cond = Condition::kOLT; negate = false; break;
            case ir::FCmp::Condition::kOGT: cond = Condition::kOGT; negate = false; break;
            case ir::FCmp::Condition::kOLE: cond = Condition::kOLE; negate = false; break;
            case ir::FCmp::Condition::kOGE: cond = Condition::kOGE; negate = false; break;
            default: abort();
        }
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src1 = getRegister(*I.lhs()),
                                  src2 = getRegister(*I.rhs());
        builder_.add(std::make_unique<FCmpSet>(8, precision, cond, dst, src1, src2));
        builder_.add(std::make_unique<Neg>(8, dst, dst));
        if (negate) {
            builder_.add(std::make_unique<XorI>(8, dst, dst, std::make_unique<IntegerImmediate>(-1)));
        }
    }

    void visitPtrToInt(const ir::PtrToInt &I) override {
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<Mov>(8, dst, src));
        int dstBitWidth = I.type()->bitSize(8);
        if (dstBitWidth < 64) {
            builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - dstBitWidth)));
            builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - dstBitWidth)));
        }
    }

    void visitIntToPtr(const ir::IntToPtr &I) override {
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<Mov>(8, dst, src));
        int srcBitWidth = I.value()->type()->bitSize(8);
        if (srcBitWidth < 64) {
            builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcBitWidth)));
            builder_.add(std::make_unique<SHRLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - srcBitWidth)));
        }
    }

    void visitBitCast(const ir::BitCast &I) override {
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.type()) && dynamic_cast<const ir::IntegerOrPointerType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type()) && dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<FMov>(Precision::kDouble, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type()) && dynamic_cast<const ir::IntegerOrPointerType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<FMovFI>(Precision::kDouble, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.type()) && dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            builder_.add(std::make_unique<FMovIF>(Precision::kDouble, std::move(dst), std::move(src)));
        } else {
            abort();
        }
    }

    void visitSelect(const ir::Select &I) override {
        BasicBlock &trueBlock = function_.append(),
                   &falseBlock = function_.append(),
                   &endBlock = function_.append();

        std::shared_ptr<Register> dst = valueMap_[&I],
                                  cond = getRegister(*I.cond()),
                                  src1 = getRegister(*I.trueValue()),
                                  src2 = getRegister(*I.falseValue());

        builder_.add(std::make_unique<CmpZBr>(8, Condition::kNEZ, cond, &trueBlock, &falseBlock));

        builder_.setPos(&trueBlock, trueBlock.end());
        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.type())) {
            builder_.add(std::make_unique<Mov>(8, dst, src1));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            builder_.add(std::make_unique<FMov>(precision, dst, src1));
        } else {
            abort();
        }
        builder_.add(std::make_unique<Br>(&endBlock));

        builder_.setPos(&falseBlock, falseBlock.end());
        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.type())) {
            builder_.add(std::make_unique<Mov>(8, dst, src2));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            builder_.add(std::make_unique<FMov>(precision, dst, src2));
        } else {
            abort();
        }
        builder_.add(std::make_unique<Br>(&endBlock));

        builder_.setPos(&endBlock, endBlock.end());
    }

    void visitAlloca(const ir::Alloca &I) override {
        RISCVRegister *fp = RISCVRegister::get("fp");
        std::shared_ptr<Register> dst = valueMap_[&I];
        StackSlot *endSlot = &function_.stackFrame().back(),
                  *slot = memoryMap_[&I];
        std::unique_ptr<Immediate> offset = std::make_unique<StackOffsetImmediate>(endSlot, slot);
        builder_.add(std::make_unique<LI>(8, dst, std::move(offset)));
        builder_.add(std::make_unique<Add>(8, dst, share(*fp), dst));
    }

    void visitLoad(const ir::Load &I) override {
        MemoryOperand src(getRegister(*I.ptr()));
        std::shared_ptr<Register> dst = valueMap_[&I];

        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.type())) {
            int width = I.type()->size(8);
            ExtensionMode extMode = width == 8 ? ExtensionMode::kNo : ExtensionMode::kSign;
            builder_.add(std::make_unique<Load>(width, std::move(dst), std::move(src), extMode));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            builder_.add(std::make_unique<FLoad>(precision, std::move(dst), std::move(src)));
        } else {
            abort();
        }
    }

    void visitStore(const ir::Store &I) override {
        MemoryOperand dst(getRegister(*I.ptr()));
        std::shared_ptr<Register> src = getRegister(*I.value());

        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.value()->type())) {
            int width = I.value()->type()->size(8);
            builder_.add(std::make_unique<Store>(width, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
            builder_.add(std::make_unique<FStore>(precision, std::move(dst), std::move(src)));
        } else {
            abort();
        }
    }

    void visitGetElementPtr(const ir::GetElementPtr &I) override {
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.ptr());

        builder_.add(std::make_unique<Mov>(8, dst, src));

        auto add = [this](std::shared_ptr<Register> dst, const ir::Value &idx, int size) {
            std::shared_ptr<Register> idxReg = getRegister(idx);
            if (size == 1) {
                builder_.add(std::make_unique<Add>(8, dst, dst, idxReg));
            } else {
                std::shared_ptr<Register> sizeReg = std::make_shared<VirtualRegister>(8);
                builder_.add(std::make_unique<LI>(8, sizeReg, std::make_unique<IntegerImmediate>(size)));
                builder_.add(std::make_unique<Mul>(8, idxReg, idxReg, sizeReg));
                builder_.add(std::make_unique<Add>(8, dst, dst, idxReg));
            }
        };

        size_t n = I.idx_size();
        std::unique_ptr<ir::Type> type = I.sourceType();

        if (n > 0) {
            add(dst, *I.idx(0), type->size(8));
            for (size_t i = 1; i < n; ++i) {
                type = static_cast<const ir::ArrayType *>(&*type)->elementType();
                add(dst, *I.idx(i), type->size(8));
            }
        }
    }

    void visitCall(const ir::Call &I) override {
        visitCallOrIndirectCall(I);
    }

    void visitIndirectCall(const ir::IndirectCall &I) override {
        visitCallOrIndirectCall(I);
    }

    void visitBr(const ir::Br &I) override {
        BasicBlock *dest = blockMap_[&*I.dest()];
        std::vector<const ir::Phi *> phis;
        for (const ir::Instruction &II : *I.dest()) {
            if (auto *phi = dynamic_cast<const ir::Phi *>(&II)) {
                phis.push_back(phi);
            }
        }
        if (!phis.empty()) {
            BasicBlockBuilder builder = builder_;
            BasicBlock &middle = function_.append();
            builder_.setPos(&middle);
            HashMap<const ir::Phi *, std::shared_ptr<Register>> tmps;
            for (const ir::Phi *phi : phis) {
                tmps.put(phi, std::make_shared<VirtualRegister>(8));
            }
            for (const ir::Phi *phi : phis) {
                std::shared_ptr<Register> dst = tmps[phi],
                                          src = getRegister(*getIncomingValue(*phi, *I.parent()));
                if (dynamic_cast<const ir::IntegerOrPointerType *>(&*phi->type())) {
                    builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                    Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                    builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                } else {
                    abort();
                }
            }
            for (const ir::Phi *phi : phis) {
                std::shared_ptr<Register> dst = valueMap_[phi],
                                          src = tmps[phi];
                if (dynamic_cast<const ir::IntegerOrPointerType *>(&*phi->type())) {
                    builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                    Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                    builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                } else {
                    abort();
                }
            }
            builder_.add(std::make_unique<Br>(dest));
            builder_ = builder;
            dest = &middle;
        }
        builder_.add(std::make_unique<Br>(dest));
    }

    void visitCondBr(const ir::CondBr &I) override {
        BasicBlock *trueDest = blockMap_[&*I.trueDest()];
        BasicBlock *falseDest = blockMap_[&*I.falseDest()];
        {
            std::vector<const ir::Phi *> phis;
            for (const ir::Instruction &II : *I.trueDest()) {
                if (auto *phi = dynamic_cast<const ir::Phi *>(&II)) {
                    phis.push_back(phi);
                }
            }
            if (!phis.empty()) {
                BasicBlockBuilder builder = builder_;
                BasicBlock &middle = function_.append();
                builder_.setPos(&middle);
                HashMap<const ir::Phi *, std::shared_ptr<Register>> tmps;
                for (const ir::Phi *phi : phis) {
                    tmps.put(phi, std::make_shared<VirtualRegister>(8));
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = tmps[phi],
                                              src = getRegister(*getIncomingValue(*phi, *I.parent()));
                    if (dynamic_cast<const ir::IntegerOrPointerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        abort();
                    }
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = valueMap_[phi],
                                              src = tmps[phi];
                    if (dynamic_cast<const ir::IntegerOrPointerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        abort();
                    }
                }
                builder_.add(std::make_unique<Br>(trueDest));
                builder_ = builder;
                trueDest = &middle;
            }
        }
        {
            std::vector<const ir::Phi *> phis;
            for (const ir::Instruction &II : *I.falseDest()) {
                if (auto *phi = dynamic_cast<const ir::Phi *>(&II)) {
                    phis.push_back(phi);
                }
            }
            if (!phis.empty()) {
                BasicBlockBuilder builder = builder_;
                BasicBlock &middle = function_.append();
                builder_.setPos(&middle);
                HashMap<const ir::Phi *, std::shared_ptr<Register>> tmps;
                for (const ir::Phi *phi : phis) {
                    tmps.put(phi, std::make_shared<VirtualRegister>(8));
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = tmps[phi],
                                              src = getRegister(*getIncomingValue(*phi, *I.parent()));
                    if (dynamic_cast<const ir::IntegerOrPointerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        abort();
                    }
                }
                for (const ir::Phi *phi : phis) {
                    std::shared_ptr<Register> dst = valueMap_[phi],
                                              src = tmps[phi];
                    if (dynamic_cast<const ir::IntegerOrPointerType *>(&*phi->type())) {
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    } else if (dynamic_cast<const ir::FloatingType *>(&*phi->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*phi->type())->precision();
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                    } else {
                        abort();
                    }
                }
                builder_.add(std::make_unique<Br>(falseDest));
                builder_ = builder;
                falseDest = &middle;
            }
        }
        if (auto *icmp = dynamic_cast<const ir::ICmp *>(&*I.cond())) {
            int width = icmp->lhs()->type()->size(8);
            std::shared_ptr<Register> src1 = getRegister(*icmp->lhs()),
                                      src2 = getRegister(*icmp->rhs());
            Condition cond;
            switch (icmp->cond()) {
                case ir::ICmp::Condition::kEQ: cond = Condition::kEQ; break;
                case ir::ICmp::Condition::kNE: cond = Condition::kNE; break;
                case ir::ICmp::Condition::kSLT: cond = Condition::kSLT; break;
                case ir::ICmp::Condition::kSGT: cond = Condition::kSGT; break;
                case ir::ICmp::Condition::kSLE: cond = Condition::kSLE; break;
                case ir::ICmp::Condition::kSGE: cond = Condition::kSGE; break;
                case ir::ICmp::Condition::kULT: cond = Condition::kULT; break;
                case ir::ICmp::Condition::kUGT: cond = Condition::kUGT; break;
                case ir::ICmp::Condition::kULE: cond = Condition::kULE; break;
                case ir::ICmp::Condition::kUGE: cond = Condition::kUGE; break;
                default: abort();
            }
            builder_.add(std::make_unique<CmpBr>(width, cond, std::move(src1), std::move(src2), trueDest, falseDest));
        } else {
            std::shared_ptr<Register> cond = getRegister(*I.cond());
            builder_.add(std::make_unique<CmpZBr>(8, Condition::kNEZ, std::move(cond), trueDest, falseDest));
        }
    }

    void visitRet(const ir::Ret &I) override {
        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.value()->type())) {
            std::shared_ptr<Register> dst = share(*riscvIntegerResultRegs()[0]),
                                      src = getRegister(*I.value());
            builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.value()->type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
            std::shared_ptr<Register> dst = share(*riscvFloatingResultRegs()[0]),
                                      src = getRegister(*I.value());
            builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
        } else {
            assert(*I.value()->type() == ir::Void());
        }
        builder_.add(std::make_unique<Br>(epilogueBlock_));
    }

    void visitPhi(const ir::Phi &) override {}

private:
    Function &function_;
    const HashMap<const ir::GlobalVar *, GlobalVar *> &globalVarMap_;
    const HashMap<const ir::Function *, Function *> &functionMap_;
    const HashMap<const ir::BasicBlock *, BasicBlock *> &blockMap_;
    const HashMap<const ir::Alloca *, StackSlot *> &memoryMap_;
    const HashMap<const ir::Value *, std::shared_ptr<Register>> &valueMap_;
    BasicBlock *epilogueBlock_;
    BasicBlockBuilder builder_;

    template <typename IInstr, typename MInstr>
    void visitBinaryIntegerArithmeticOperator(const IInstr &I) {
        int bitWidth = I.type()->bitSize(8);
        ExtensionMode extMode = bitWidth == 64 ? ExtensionMode::kNo : ExtensionMode::kSign;
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src1 = getRegister(*I.lhs()),
                                  src2 = getRegister(*I.rhs());

        if (bitWidth == 32) {
            builder_.add(std::make_unique<MInstr>(4, dst, src1, src2, extMode));
        } else {
            builder_.add(std::make_unique<MInstr>(8, dst, src1, src2, extMode));
            if (bitWidth < 64) {
                builder_.add(std::make_unique<SHLI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - bitWidth)));
                builder_.add(std::make_unique<SHRAI>(8, dst, dst, std::make_unique<IntegerImmediate>(64 - bitWidth)));
            }
        }
    }

    template <typename IInstr, typename MInstr>
    void visitBinaryIntegerBitwiseOperator(const IInstr &I) {
        int width = 8;
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src1 = getRegister(*I.lhs()),
                                  src2 = getRegister(*I.rhs());
        builder_.add(std::make_unique<MInstr>(width, std::move(dst), std::move(src1), std::move(src2)));
    }

    template <typename IInstr, typename MInstr>
    void visitUnaryFloatingArithmeticOperator(const IInstr &I) {
        Precision precision = static_cast<const ir::FloatingType *>(&*I.value()->type())->precision();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src = getRegister(*I.value());
        builder_.add(std::make_unique<MInstr>(precision, std::move(dst), std::move(src)));
    }

    template <typename IInstr, typename MInstr>
    void visitBinaryFloatingArithmeticOperator(const IInstr &I) {
        Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
        std::shared_ptr<Register> dst = valueMap_[&I],
                                  src1 = getRegister(*I.lhs()),
                                  src2 = getRegister(*I.rhs());
        builder_.add(std::make_unique<MInstr>(precision, std::move(dst), std::move(src1), std::move(src2)));
    }

    template <typename IInstr>
    void visitCallOrIndirectCall(const IInstr &I) {
        RISCVRegister *sp = RISCVRegister::get("sp");

        size_t numIntegerArgs = 0,
               numFloatingArgs = 0;
        std::vector<const ir::Value *> stackArgs;

        for (const ir::Use<ir::Value> &arg : args(I)) {
            if (ir::functionType(I)->isVarArgs()) {
                if (numIntegerArgs < 8) {
                    std::shared_ptr<Register> dst = share(*riscvIntegerArgRegs()[numIntegerArgs]),
                                              src = getRegister(*arg);
                    if (dynamic_cast<const ir::FloatingType *>(&*arg->type())) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*arg->type())->precision();
                        std::shared_ptr<Register> tmp = std::make_shared<VirtualRegister>(8);
                        builder_.add(std::make_unique<FMovIF>(precision, tmp, src));
                        src = std::move(tmp);
                    }
                    builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                    ++numIntegerArgs;
                } else {
                    stackArgs.push_back(&*arg);
                }
            } else {
                if (dynamic_cast<const ir::IntegerOrPointerType *>(&*arg->type())) {
                    if (numIntegerArgs < 8) {
                        std::shared_ptr<Register> dst = share(*riscvIntegerArgRegs()[numIntegerArgs]),
                                                  src = getRegister(*arg);
                        builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
                        ++numIntegerArgs;
                    } else {
                        stackArgs.push_back(&*arg);
                    }
                } else if (dynamic_cast<const ir::FloatingType *>(&*arg->type())) {
                    if (numFloatingArgs < 8) {
                        Precision precision = static_cast<const ir::FloatingType *>(&*arg->type())->precision();
                        std::shared_ptr<Register> dst = share(*riscvFloatingArgRegs()[numFloatingArgs]),
                                                  src = getRegister(*arg);
                        builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
                        ++numFloatingArgs;
                    } else {
                        stackArgs.push_back(&*arg);
                    }
                } else {
                    abort();
                }
            }
        }

        if (!stackArgs.empty()) {
            int n = stackArgs.size();
            builder_.add(std::make_unique<AddI>(8, share(*sp), share(*sp), std::make_unique<IntegerImmediate>(-(n * 8 + 15) / 16 * 16)));
            for (int i = 0; i < n; ++i) {
                MemoryOperand dst(share(*sp), std::make_unique<IntegerImmediate>(i * 8));
                std::shared_ptr<Register> src = getRegister(*stackArgs[i]);
                if (dynamic_cast<const ir::IntegerOrPointerType *>(&*stackArgs[i]->type())) {
                    builder_.add(std::make_unique<Store>(8, std::move(dst), std::move(src)));
                } else if (dynamic_cast<const ir::FloatingType *>(&*stackArgs[i]->type())) {
                    Precision precision = static_cast<const ir::FloatingType *>(&*stackArgs[i]->type())->precision();
                    builder_.add(std::make_unique<FStore>(precision, std::move(dst), std::move(src)));
                } else {
                    abort();
                }
            }
        }

        if constexpr (std::is_same_v<IInstr, ir::Call>) {
            Function *callee = functionMap_[&*I.callee()];
            builder_.add(std::make_unique<RISCVCall>(callee, numIntegerArgs, numFloatingArgs));
        }
        if constexpr (std::is_same_v<IInstr, ir::IndirectCall>) {
            std::shared_ptr<Register> callee = getRegister(*I.callee());
            builder_.add(std::make_unique<RISCVJALR>(callee, numIntegerArgs, numFloatingArgs));
        }

        if (!stackArgs.empty()) {
            int n = stackArgs.size();
            builder_.add(std::make_unique<AddI>(8, share(*sp), share(*sp), std::make_unique<IntegerImmediate>((n * 8 + 15) / 16 * 16)));
        }

        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*I.type())) {
            std::shared_ptr<Register> dst = valueMap_[&I],
                                      src = share(*riscvIntegerResultRegs()[0]);
            builder_.add(std::make_unique<Mov>(8, std::move(dst), std::move(src)));
        } else if (dynamic_cast<const ir::FloatingType *>(&*I.type())) {
            Precision precision = static_cast<const ir::FloatingType *>(&*I.type())->precision();
            std::shared_ptr<Register> dst = valueMap_[&I],
                                      src = share(*riscvFloatingResultRegs()[0]);
            builder_.add(std::make_unique<FMov>(precision, std::move(dst), std::move(src)));
        } else {
            assert(*I.type() == ir::Void());
        }
    }

    std::shared_ptr<Register> getRegister(const ir::Value &value) {
        if (dynamic_cast<const ir::Argument *>(&value) || dynamic_cast<const ir::Instruction *>(&value)) {
            return valueMap_[&value];
        }
        if (auto *G = dynamic_cast<const ir::GlobalVar *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>(8);
            GlobalVar *ptr = globalVarMap_[G];
            builder_.add(std::make_unique<LA>(8, reg, ptr));
            return reg;
        }
        if (auto *F = dynamic_cast<const ir::Function *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>(8);
            Function *ptr = functionMap_[F];
            builder_.add(std::make_unique<LA>(8, reg, ptr));
            return reg;
        }
        if (auto *C = dynamic_cast<const ir::IntegerConstant *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>(8);
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(C->signExtendedValue());
            builder_.add(std::make_unique<LI>(8, reg, std::move(imm)));
            return reg;
        }
        if (auto *C = dynamic_cast<const ir::FloatingConstant *>(&value)) {
            std::shared_ptr<Register> gpr = std::make_shared<VirtualRegister>(8),
                                      fpr = std::make_shared<VirtualRegister>(8);
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(C->bitPattern());
            builder_.add(std::make_unique<LI>(8, gpr, std::move(imm)));
            Precision precision = static_cast<const ir::FloatingType *>(&*C->type())->precision();
            builder_.add(std::make_unique<FMovFI>(precision, fpr, gpr));
            return fpr;
        }
        if (dynamic_cast<const ir::NullPtrConstant *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>(8);
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(0);
            builder_.add(std::make_unique<LI>(8, reg, std::move(imm)));
            return reg;
        }
        if (dynamic_cast<const ir::PoisonValue *>(&value)) {
            std::shared_ptr<Register> reg = std::make_shared<VirtualRegister>(8);
            return reg;
        }
        abort();
    }
};

} // namespace

class RISCVMIRGen::Impl {
public:
    Impl(const ir::Module *IM, Module *MM)
        : IM_(IM), MM_(MM) {}

    void emit() {
        for (const ir::GlobalVar &IG : globalVars(*IM_)) {
            if (IG.isDeclaration()) {
                GlobalVar &MG = MM_->appendGlobalVar(std::make_unique<GlobalVar>(IG.name(), IG.linkage()));
                globalVarMap_.put(&IG, &MG);
            } else {
                GlobalVar &MG = MM_->appendGlobalVar(std::make_unique<GlobalVar>(IG.name(), IG.linkage(), IG.isConstant(), IG.valueType()->alignment(8), nullptr));
                globalVarMap_.put(&IG, &MG);
            }
        }
        for (const ir::Function &IF : functions(*IM_)) {
            Function &MF = MM_->appendFunction(std::make_unique<Function>(IF.name(), IF.linkage()));
            functionMap_.put(&IF, &MF);
        }
        for (auto &[IG, MG] : globalVarMap_) {
            if (!IG->isDeclaration()) {
                emitGlobalVar(*IG, *MG);
            }
        }
        for (auto &[IF, MF] : functionMap_) {
            if (!IF->empty()) {
                emitFunction(*IF, *MF);
            }
        }
    }

private:
    const ir::Module *IM_;
    Module *MM_;
    HashMap<const ir::GlobalVar *, GlobalVar *> globalVarMap_;
    HashMap<const ir::Function *, Function *> functionMap_;

    void emitGlobalVar(const ir::GlobalVar &IG, GlobalVar &MG) {
        MG.setInitializer(emitConstant(IG.initializer(), globalVarMap_, functionMap_));
    }

    void emitFunction(const ir::Function &IF, Function &MF) {
        RISCVRegister *ra = RISCVRegister::get("ra"),
                      *sp = RISCVRegister::get("sp"),
                      *fp = RISCVRegister::get("fp"),
                      *t6 = RISCVRegister::get("t6");

        BasicBlock &prologueBlock = MF.append();

        HashMap<const ir::BasicBlock *, BasicBlock *> blockMap;
        for (const ir::BasicBlock &IB : IF) {
            blockMap.put(&IB, &MF.append());
        }

        BasicBlock &epilogueBlock = MF.append();

        //  high +------------+  <-- endSlot
        //       |            |
        //       | stackFrame |
        //       |            |
        //   low +------------+  <-- startSlot

        StackSlot &startSlot = MF.stackFrame().append(0, 16);
        StackSlot &raSlot = MF.stackFrame().append(8, 8);
        StackSlot &fpSlot = MF.stackFrame().append(8, 8);

        HashMap<const ir::Alloca *, StackSlot *> memoryMap;
        for (const ir::BasicBlock &IB : IF) {
            for (const ir::Instruction &II : IB) {
                if (auto *alloca = dynamic_cast<const ir::Alloca *>(&II)) {
                    int size = alloca->allocatedType()->size(8);
                    int alignment = alloca->allocatedType()->alignment(8);
                    memoryMap.put(alloca, &MF.stackFrame().append(size, alignment));
                }
            }
        }

        StackSlot &endSlot = MF.stackFrame().append(0, 16);

        prologueBlock.append(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(&startSlot, &endSlot)));
        prologueBlock.append(std::make_unique<Sub>(8, share(*sp), share(*sp), share(*t6)));

        prologueBlock.append(std::make_unique<Store>(
            8, MemoryOperand(share(*sp), std::make_unique<StackOffsetImmediate>(&startSlot, &raSlot)), share(*ra)
        ));

        prologueBlock.append(std::make_unique<Store>(
            8, MemoryOperand(share(*sp), std::make_unique<StackOffsetImmediate>(&startSlot, &fpSlot)), share(*fp)
        ));

        prologueBlock.append(std::make_unique<Marker>(kSave));

        prologueBlock.append(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(&startSlot, &endSlot)));
        prologueBlock.append(std::make_unique<Add>(8, share(*fp), share(*sp), share(*t6)));

        prologueBlock.append(std::make_unique<Br>(blockMap[&IF.entry()]));

        epilogueBlock.append(std::make_unique<Load>(
            8, share(*ra), MemoryOperand(share(*sp), std::make_unique<StackOffsetImmediate>(&startSlot, &raSlot))
        ));

        epilogueBlock.append(std::make_unique<Load>(
            8, share(*fp), MemoryOperand(share(*sp), std::make_unique<StackOffsetImmediate>(&startSlot, &fpSlot))
        ));

        epilogueBlock.append(std::make_unique<Marker>(kRestore));

        epilogueBlock.append(std::make_unique<LI>(8, share(*t6), std::make_unique<StackOffsetImmediate>(&startSlot, &endSlot)));
        epilogueBlock.append(std::make_unique<Add>(8, share(*sp), share(*sp), share(*t6)));

        int numIntegerResults = 0,
            numFloatingResults = 0;
        std::unique_ptr<ir::Type> returnType = IF.functionType()->returnType();
        if (dynamic_cast<const ir::IntegerOrPointerType *>(&*returnType)) {
            ++numIntegerResults;
        } else if (dynamic_cast<const ir::FloatingType *>(&*returnType)) {
            ++numFloatingResults;
        } else {
            assert(*returnType == ir::Void());
        }
        epilogueBlock.append(std::make_unique<RISCVRet>(numIntegerResults, numFloatingResults));

        HashMap<const ir::Value *, std::shared_ptr<Register>> valueMap;
        size_t numIntegerArgs = 0,
               numFloatingArgs = 0,
               numStackArgs = 0;
        for (const ir::Argument &arg : args(IF)) {
            if (dynamic_cast<const ir::IntegerOrPointerType *>(&*arg.type())) {
                if (numIntegerArgs < 8) {
                    std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>(8),
                                              src = share(*riscvIntegerArgRegs()[numIntegerArgs]);
                    blockMap[&IF.entry()]->append(std::make_unique<Mov>(8, dst, src));
                    valueMap.put(&arg, dst);
                    ++numIntegerArgs;
                } else {
                    std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>(8);
                    MemoryOperand src(share(*fp), std::make_unique<IntegerImmediate>(numStackArgs * 8));
                    blockMap[&IF.entry()]->append(std::make_unique<Load>(8, dst, std::move(src)));
                    valueMap.put(&arg, dst);
                    ++numStackArgs;
                }
            } else if (dynamic_cast<const ir::FloatingType *>(&*arg.type())) {
                Precision precision = static_cast<const ir::FloatingType *>(&*arg.type())->precision();
                if (numFloatingArgs < 8) {
                    std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>(8),
                                              src = share(*riscvFloatingArgRegs()[numFloatingArgs]);
                    blockMap[&IF.entry()]->append(std::make_unique<FMov>(precision, dst, src));
                    valueMap.put(&arg, dst);
                    ++numFloatingArgs;
                } else {
                    std::shared_ptr<Register> dst = std::make_shared<VirtualRegister>(8);
                    MemoryOperand src(share(*fp), std::make_unique<IntegerImmediate>(numStackArgs * 8));
                    blockMap[&IF.entry()]->append(std::make_unique<FLoad>(precision, dst, std::move(src)));
                    valueMap.put(&arg, dst);
                    ++numStackArgs;
                }
            } else {
                abort();
            }
        }
        for (const ir::BasicBlock &IB : IF) {
            for (const ir::Instruction &II : IB) {
                if (*II.type() != ir::Void()) {
                    valueMap.put(&II, std::make_shared<VirtualRegister>(8));
                }
            }
        }

        InstructionVisitorImpl visitor(MF, globalVarMap_, functionMap_, blockMap, memoryMap, valueMap, &epilogueBlock);
        for (const ir::BasicBlock &IB : IF) {
            BasicBlock *MB = blockMap[&IB];
            visitor.builder().setPos(MB);
            for (const ir::Instruction &II : IB) {
                II.accept(visitor);
            }
        }
    }
};

RISCVMIRGen::RISCVMIRGen(const ir::Module *IM, Module *MM)
    : impl_(std::make_unique<Impl>(IM, MM)) {}

RISCVMIRGen::~RISCVMIRGen() = default;

RISCVMIRGen::RISCVMIRGen(RISCVMIRGen &&) noexcept = default;

RISCVMIRGen &RISCVMIRGen::operator=(RISCVMIRGen &&) noexcept = default;

void RISCVMIRGen::emit() {
    impl_->emit();
}
