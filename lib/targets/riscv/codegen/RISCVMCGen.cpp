#include "mini-llvm/targets/riscv/codegen/RISCVMCGen.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/ops/SExt.h"
#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/FragmentBuilder.h"
#include "mini-llvm/mc/ImmediateOperand.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/mc/LabelOperand.h"
#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/Constant/I16ArrayConstant.h"
#include "mini-llvm/mir/Constant/I16Constant.h"
#include "mini-llvm/mir/Constant/I32ArrayConstant.h"
#include "mini-llvm/mir/Constant/I32Constant.h"
#include "mini-llvm/mir/Constant/I64ArrayConstant.h"
#include "mini-llvm/mir/Constant/I64Constant.h"
#include "mini-llvm/mir/Constant/I8ArrayConstant.h"
#include "mini-llvm/mir/Constant/I8Constant.h"
#include "mini-llvm/mir/Constant/PtrConstant.h"
#include "mini-llvm/mir/Constant/ZeroConstant.h"
#include "mini-llvm/mir/ConstantVisitor.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/FunctionOperand.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/mir/GlobalVarOperand.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/AddI.h"
#include "mini-llvm/mir/Instruction/And.h"
#include "mini-llvm/mir/Instruction/AndI.h"
#include "mini-llvm/mir/Instruction/BinaryOperator.h"
#include "mini-llvm/mir/Instruction/BinaryOperatorI.h"
#include "mini-llvm/mir/Instruction/Br.h"
#include "mini-llvm/mir/Instruction/CmpBr.h"
#include "mini-llvm/mir/Instruction/CmpSet.h"
#include "mini-llvm/mir/Instruction/CmpZBr.h"
#include "mini-llvm/mir/Instruction/CmpZSet.h"
#include "mini-llvm/mir/Instruction/FAdd.h"
#include "mini-llvm/mir/Instruction/FBinaryOperator.h"
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
#include "mini-llvm/mir/Instruction/FUnaryOperator.h"
#include "mini-llvm/mir/Instruction/LA.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Load.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Instruction/Mul.h"
#include "mini-llvm/mir/Instruction/Not.h"
#include "mini-llvm/mir/Instruction/Or.h"
#include "mini-llvm/mir/Instruction/OrI.h"
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
#include "mini-llvm/mir/Instruction/UnaryOperator.h"
#include "mini-llvm/mir/Instruction/URem.h"
#include "mini-llvm/mir/Instruction/Xor.h"
#include "mini-llvm/mir/Instruction/XorI.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/targets/riscv/mc/RISCVDataDirective.h"
#include "mini-llvm/targets/riscv/mc/RISCVInstruction.h"
#include "mini-llvm/targets/riscv/mc/RISCVLabelDirective.h"
#include "mini-llvm/targets/riscv/mc/RISCVMemoryOperand.h"
#include "mini-llvm/targets/riscv/mc/RISCVOperation.h"
#include "mini-llvm/targets/riscv/mc/RISCVRegisterOperand.h"
#include "mini-llvm/targets/riscv/mc/RISCVZeroDirective.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVCall.h"
#include "mini-llvm/targets/riscv/mir/Instruction/RISCVRet.h"
#include "mini-llvm/targets/riscv/mir/RISCVInstructionVisitor.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

using namespace mini_llvm;
using namespace mini_llvm::mc;

namespace {

class ConstantVisitorImpl final : public mir::ConstantVisitor {
public:
    FragmentBuilder &builder() {
        return builder_;
    }

    const FragmentBuilder &builder() const {
        return builder_;
    }

    void visitI8Constant(const mir::I8Constant &C) override {
        visitScalarConstant<mir::I8Constant, 1>(C);
    }

    void visitI16Constant(const mir::I16Constant &C) override {
        visitScalarConstant<mir::I16Constant, 2>(C);
    }

    void visitI32Constant(const mir::I32Constant &C) override {
        visitScalarConstant<mir::I32Constant, 4>(C);
    }

    void visitI64Constant(const mir::I64Constant &C) override {
        visitScalarConstant<mir::I64Constant, 8>(C);
    }

    void visitI8ArrayConstant(const mir::I8ArrayConstant &C) override {
        visitAggregateConstant<mir::I8ArrayConstant, 1>(C);
    }

    void visitI16ArrayConstant(const mir::I16ArrayConstant &C) override {
        visitAggregateConstant<mir::I16ArrayConstant, 2>(C);
    }

    void visitI32ArrayConstant(const mir::I32ArrayConstant &C) override {
        visitAggregateConstant<mir::I32ArrayConstant, 4>(C);
    }

    void visitI64ArrayConstant(const mir::I64ArrayConstant &C) override {
        visitAggregateConstant<mir::I64ArrayConstant, 8>(C);
    }

    void visitZeroConstant(const mir::ZeroConstant &C) override {
        builder_.add(std::make_unique<RISCVZeroDirective>(C.size()));
    }

    void visitPtrConstant(const mir::PtrConstant &C) override {
        builder_.add(std::make_unique<RISCVLabelDirective>(C.ptr()->name()));
    }

private:
    FragmentBuilder builder_;

    template <typename MConst, int Width>
    void visitScalarConstant(const MConst &C) {
        builder_.add(std::make_unique<RISCVDataDirective>(Width, ops::SExt<int64_t>()(C.value())));
    }

    template <typename MConst, int Width>
    void visitAggregateConstant(const MConst &C) {
        for (auto element : C.elements()) {
            builder_.add(std::make_unique<RISCVDataDirective>(Width, ops::SExt<int64_t>()(element)));
        }
    }
};

class RISCVInstructionVisitorImpl final : public mir::RISCVInstructionVisitor {
public:
    FragmentBuilder &builder() {
        return builder_;
    }

    const FragmentBuilder &builder() const {
        return builder_;
    }

    void visitMov(const mir::Mov &I) override {
        visitUnaryBitwiseOperator<RISCV_Mv>(I);
    }

    void visitNot(const mir::Not &I) override {
        visitUnaryBitwiseOperator<RISCV_Not>(I);
    }

    void visitAdd(const mir::Add &I) override {
        visitBinaryOperator<RISCV_AddW, RISCV_Add>(I);
    }

    void visitSub(const mir::Sub &I) override {
        visitBinaryOperator<RISCV_SubW, RISCV_Sub>(I);
    }

    void visitMul(const mir::Mul &I) override {
        visitBinaryOperator<RISCV_MulW, RISCV_Mul>(I);
    }

    void visitSDiv(const mir::SDiv &I) override {
        visitBinaryOperator<RISCV_DivW, RISCV_Div>(I);
    }

    void visitUDiv(const mir::UDiv &I) override {
        visitBinaryOperator<RISCV_DivUW, RISCV_DivU>(I);
    }

    void visitSRem(const mir::SRem &I) override {
        visitBinaryOperator<RISCV_RemW, RISCV_Rem>(I);
    }

    void visitURem(const mir::URem &I) override {
        visitBinaryOperator<RISCV_RemUW, RISCV_RemU>(I);
    }

    void visitAnd(const mir::And &I) override {
        visitBinaryBitwiseOperator<RISCV_And>(I);
    }

    void visitOr(const mir::Or &I) override {
        visitBinaryBitwiseOperator<RISCV_Or>(I);
    }

    void visitXor(const mir::Xor &I) override {
        visitBinaryBitwiseOperator<RISCV_Xor>(I);
    }

    void visitSHL(const mir::SHL &I) override {
        visitBinaryOperator<RISCV_SLLW, RISCV_SLL>(I);
    }

    void visitSHRL(const mir::SHRL &I) override {
        visitBinaryOperator<RISCV_SRLW, RISCV_SRL>(I);
    }

    void visitSHRA(const mir::SHRA &I) override {
        visitBinaryOperator<RISCV_SRAW, RISCV_SRA>(I);
    }

    void visitAddI(const mir::AddI &I) override {
        visitBinaryOperatorI<RISCV_AddIW, RISCV_AddI>(I);
    }

    void visitAndI(const mir::AndI &I) override {
        visitBinaryBitwiseOperatorI<RISCV_AndI>(I);
    }

    void visitOrI(const mir::OrI &I) override {
        visitBinaryBitwiseOperatorI<RISCV_OrI>(I);
    }

    void visitXorI(const mir::XorI &I) override {
        visitBinaryBitwiseOperatorI<RISCV_XorI>(I);
    }

    void visitSHLI(const mir::SHLI &I) override {
        visitBinaryOperatorI<RISCV_SLLIW, RISCV_SLLI>(I);
    }

    void visitSHRLI(const mir::SHRLI &I) override {
        visitBinaryOperatorI<RISCV_SRLIW, RISCV_SRLI>(I);
    }

    void visitSHRAI(const mir::SHRAI &I) override {
        visitBinaryOperatorI<RISCV_SRAIW, RISCV_SRAI>(I);
    }

    void visitSExt(const mir::SExt &I) override {
        assert(I.dstWidth() == 8 && I.srcWidth() == 4);
        int opcode = RISCV_SExtW;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFMov(const mir::FMov &I) override {
        visitFUnaryOperator<RISCV_FMvS, RISCV_FMvD>(I);
    }

    void visitFNeg(const mir::FNeg &I) override {
        visitFUnaryOperator<RISCV_FNegS, RISCV_FNegD>(I);
    }

    void visitFAdd(const mir::FAdd &I) override {
        visitFBinaryOperator<RISCV_FAddS, RISCV_FAddD>(I);
    }

    void visitFSub(const mir::FSub &I) override {
        visitFBinaryOperator<RISCV_FSubS, RISCV_FSubD>(I);
    }

    void visitFMul(const mir::FMul &I) override {
        visitFBinaryOperator<RISCV_FMulS, RISCV_FMulD>(I);
    }

    void visitFDiv(const mir::FDiv &I) override {
        visitFBinaryOperator<RISCV_FDivS, RISCV_FDivD>(I);
    }

    void visitFCvt(const mir::FCvt &I) override {
        int opcode;
        if (I.dstPrecision() == Precision::kSingle && I.srcPrecision() == Precision::kDouble) {
            opcode = RISCV_FCvtSD;
        } else if (I.dstPrecision() == Precision::kDouble && I.srcPrecision() == Precision::kSingle) {
            opcode = RISCV_FCvtDS;
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFCvtFS(const mir::FCvtFS &I) override {
        int opcode;
        if (I.dstPrecision() == Precision::kSingle && I.srcWidth() == 4) {
            opcode = RISCV_FCvtSW;
        } else if (I.dstPrecision() == Precision::kSingle && I.srcWidth() == 8) {
            opcode = RISCV_FCvtSL;
        } else if (I.dstPrecision() == Precision::kDouble && I.srcWidth() == 4) {
            opcode = RISCV_FCvtDW;
        } else if (I.dstPrecision() == Precision::kDouble && I.srcWidth() == 8) {
            opcode = RISCV_FCvtDL;
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFCvtFU(const mir::FCvtFU &I) override {
        int opcode;
        if (I.dstPrecision() == Precision::kSingle && I.srcWidth() == 4) {
            opcode = RISCV_FCvtSWU;
        } else if (I.dstPrecision() == Precision::kSingle && I.srcWidth() == 8) {
            opcode = RISCV_FCvtSLU;
        } else if (I.dstPrecision() == Precision::kDouble && I.srcWidth() == 4) {
            opcode = RISCV_FCvtDWU;
        } else if (I.dstPrecision() == Precision::kDouble && I.srcWidth() == 8) {
            opcode = RISCV_FCvtDLU;
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFCvtSF(const mir::FCvtSF &I) override {
        int opcode;
        if (I.dstWidth() == 4 && I.srcPrecision() == Precision::kSingle) {
            opcode = RISCV_FCvtWS;
        } else if (I.dstWidth() == 4 && I.srcPrecision() == Precision::kDouble) {
            opcode = RISCV_FCvtWD;
        } else if (I.dstWidth() == 8 && I.srcPrecision() == Precision::kSingle) {
            opcode = RISCV_FCvtLS;
        } else if (I.dstWidth() == 8 && I.srcPrecision() == Precision::kDouble) {
            opcode = RISCV_FCvtLD;
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFCvtUF(const mir::FCvtUF &I) override {
        int opcode;
        if (I.dstWidth() == 4 && I.srcPrecision() == Precision::kSingle) {
            opcode = RISCV_FCvtWUS;
        } else if (I.dstWidth() == 4 && I.srcPrecision() == Precision::kDouble) {
            opcode = RISCV_FCvtWUD;
        } else if (I.dstWidth() == 8 && I.srcPrecision() == Precision::kSingle) {
            opcode = RISCV_FCvtLUS;
        } else if (I.dstWidth() == 8 && I.srcPrecision() == Precision::kDouble) {
            opcode = RISCV_FCvtLUD;
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFMovFI(const mir::FMovFI &I) override {
        int opcode;
        switch (I.precision()) {
            case Precision::kSingle: opcode = RISCV_FMvWX; break;
            case Precision::kDouble: opcode = RISCV_FMvDX; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFMovIF(const mir::FMovIF &I) override {
        int opcode;
        switch (I.precision()) {
            case Precision::kSingle: opcode = RISCV_FMvXW; break;
            case Precision::kDouble: opcode = RISCV_FMvXD; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitLoad(const mir::Load &I) override {
        int opcode;
        if (I.extensionMode() == ExtensionMode::kNo) {
            if (I.width() == 8) {
                opcode = RISCV_LD;
            } else {
                std::unreachable();
            }
        } else if (I.extensionMode() == ExtensionMode::kSign) {
            switch (I.width()) {
                case 1: opcode = RISCV_LB; break;
                case 2: opcode = RISCV_LH; break;
                case 4: opcode = RISCV_LW; break;
                default: std::unreachable();
            }
        } else if (I.extensionMode() == ExtensionMode::kZero) {
            switch (I.width()) {
                case 1: opcode = RISCV_LBU; break;
                case 2: opcode = RISCV_LHU; break;
                case 4: opcode = RISCV_LWU; break;
                default: std::unreachable();
            }
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitStore(const mir::Store &I) override {
        int opcode;
        switch (I.width()) {
            case 1: opcode = RISCV_SB; break;
            case 2: opcode = RISCV_SH; break;
            case 4: opcode = RISCV_SW; break;
            case 8: opcode = RISCV_SD; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.src()));
        operands.push_back(makeOperand(I.dst()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFLoad(const mir::FLoad &I) override {
        int opcode;
        switch (I.precision()) {
            case Precision::kSingle: opcode = RISCV_FLW; break;
            case Precision::kDouble: opcode = RISCV_FLD; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFStore(const mir::FStore &I) override {
        int opcode;
        switch (I.precision()) {
            case Precision::kSingle: opcode = RISCV_FSW; break;
            case Precision::kDouble: opcode = RISCV_FSD; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.src()));
        operands.push_back(makeOperand(I.dst()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitLA(const mir::LA &I) override {
        assert(I.width() == 8);
        int opcode = RISCV_LA;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitLI(const mir::LI &I) override {
        assert(I.width() == 8);
        int opcode = RISCV_LI;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitCmpSet(const mir::CmpSet &I) override {
        assert(I.dstWidth() == 8);
        int opcode;
        switch (I.cond()) {
            case mir::Condition::kSLT: opcode = RISCV_SLT; break;
            case mir::Condition::kSGT: opcode = RISCV_SGT; break;
            case mir::Condition::kULT: opcode = RISCV_SLTU; break;
            case mir::Condition::kUGT: opcode = RISCV_SGTU; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitCmpZSet(const mir::CmpZSet &I) override {
        assert(I.dstWidth() == 8);
        int opcode;
        switch (I.cond()) {
            case mir::Condition::kEQZ: opcode = RISCV_SEQZ; break;
            case mir::Condition::kNEZ: opcode = RISCV_SNEZ; break;
            case mir::Condition::kSLTZ: opcode = RISCV_SLTZ; break;
            case mir::Condition::kSGTZ: opcode = RISCV_SGTZ; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitFCmpSet(const mir::FCmpSet &I) override {
        assert(I.dstWidth() == 8);
        int opcode;
        if (I.srcPrecision() == Precision::kSingle) {
            switch (I.cond()) {
                case mir::Condition::kOEQ: opcode = RISCV_FEQS; break;
                case mir::Condition::kOLT: opcode = RISCV_FLTS; break;
                case mir::Condition::kOGT: opcode = RISCV_FGTS; break;
                case mir::Condition::kOLE: opcode = RISCV_FLES; break;
                case mir::Condition::kOGE: opcode = RISCV_FGES; break;
                default: std::unreachable();
            }
        } else if (I.srcPrecision() == Precision::kDouble) {
            switch (I.cond()) {
                case mir::Condition::kOEQ: opcode = RISCV_FEQD; break;
                case mir::Condition::kOLT: opcode = RISCV_FLTD; break;
                case mir::Condition::kOGT: opcode = RISCV_FGTD; break;
                case mir::Condition::kOLE: opcode = RISCV_FLED; break;
                case mir::Condition::kOGE: opcode = RISCV_FGED; break;
                default: std::unreachable();
            }
        } else {
            std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitBr(const mir::Br &I) override {
        int opcode = RISCV_J;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dest()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitCmpBr(const mir::CmpBr &I) override {
        int opcode1;
        switch (I.cond()) {
            case mir::Condition::kEQ: opcode1 = RISCV_BNE; break;
            case mir::Condition::kNE: opcode1 = RISCV_BEQ; break;
            case mir::Condition::kSLT: opcode1 = RISCV_BGE; break;
            case mir::Condition::kSGT: opcode1 = RISCV_BLE; break;
            case mir::Condition::kSLE: opcode1 = RISCV_BGT; break;
            case mir::Condition::kSGE: opcode1 = RISCV_BLT; break;
            case mir::Condition::kULT: opcode1 = RISCV_BGEU; break;
            case mir::Condition::kUGT: opcode1 = RISCV_BLEU; break;
            case mir::Condition::kULE: opcode1 = RISCV_BGTU; break;
            case mir::Condition::kUGE: opcode1 = RISCV_BLTU; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands1;
        operands1.push_back(makeOperand(I.src1()));
        operands1.push_back(makeOperand(I.src2()));
        operands1.push_back(makeOperand(I.falseDest()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode1, std::move(operands1)));

        int opcode2 = RISCV_J;
        std::vector<std::unique_ptr<Operand>> operands2;
        operands2.push_back(makeOperand(I.trueDest()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode2, std::move(operands2)));
    }

    void visitCmpZBr(const mir::CmpZBr &I) override {
        int opcode1;
        switch (I.cond()) {
            case mir::Condition::kEQZ: opcode1 = RISCV_BNEZ; break;
            case mir::Condition::kNEZ: opcode1 = RISCV_BEQZ; break;
            case mir::Condition::kSLTZ: opcode1 = RISCV_BGEZ; break;
            case mir::Condition::kSGTZ: opcode1 = RISCV_BLEZ; break;
            case mir::Condition::kSLEZ: opcode1 = RISCV_BGTZ; break;
            case mir::Condition::kSGEZ: opcode1 = RISCV_BLTZ; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands1;
        operands1.push_back(makeOperand(I.src()));
        operands1.push_back(makeOperand(I.falseDest()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode1, std::move(operands1)));

        int opcode2 = RISCV_J;
        std::vector<std::unique_ptr<Operand>> operands2;
        operands2.push_back(makeOperand(I.trueDest()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode2, std::move(operands2)));
    }

    void visitRISCVCall(const mir::RISCVCall &I) override {
        int opcode = RISCV_Call;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.callee()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    void visitRISCVRet(const mir::RISCVRet &) override {
        builder_.add(std::make_unique<RISCVInstruction>(RISCV_Ret));
    }

private:
    FragmentBuilder builder_;

    template <int Opcode8>
    void visitUnaryBitwiseOperator(const mir::UnaryOperator &I) {
        assert(I.width() == 8);
        assert(I.extensionMode() == ExtensionMode::kNo);
        int opcode = Opcode8;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    template <int Opcode4, int Opcode8>
    void visitBinaryOperator(const mir::BinaryOperator &I) {
        int opcode;
        switch (I.width()) {
            case 4: opcode = Opcode4; break;
            case 8: opcode = Opcode8; break;
            default: std::unreachable();
        }
#ifndef NDEBUG
        if (I.width() == 8) {
            assert(I.extensionMode() == ExtensionMode::kNo);
        } else {
            assert(I.extensionMode() == ExtensionMode::kSign);
        }
#endif
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    template <int Opcode8>
    void visitBinaryBitwiseOperator(const mir::BinaryOperator &I) {
        assert(I.width() == 8);
        assert(I.extensionMode() == ExtensionMode::kNo);
        int opcode = Opcode8;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    template <int Opcode4, int Opcode8>
    void visitBinaryOperatorI(const mir::BinaryOperatorI &I) {
        int opcode;
        switch (I.width()) {
            case 4: opcode = Opcode4; break;
            case 8: opcode = Opcode8; break;
            default: std::unreachable();
        }
#ifndef NDEBUG
        if (I.width() == 8) {
            assert(I.extensionMode() == ExtensionMode::kNo);
        } else {
            assert(I.extensionMode() == ExtensionMode::kSign);
        }
#endif
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    template <int Opcode8>
    void visitBinaryBitwiseOperatorI(const mir::BinaryOperatorI &I) {
        assert(I.width() == 8);
        assert(I.extensionMode() == ExtensionMode::kNo);
        int opcode = Opcode8;
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    template <int OpcodeS, int OpcodeD>
    void visitFUnaryOperator(const mir::FUnaryOperator &I) {
        int opcode;
        switch (I.precision()) {
            case Precision::kSingle: opcode = OpcodeS; break;
            case Precision::kDouble: opcode = OpcodeD; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    template <int OpcodeS, int OpcodeD>
    void visitFBinaryOperator(const mir::FBinaryOperator &I) {
        int opcode;
        switch (I.precision()) {
            case Precision::kSingle: opcode = OpcodeS; break;
            case Precision::kDouble: opcode = OpcodeD; break;
            default: std::unreachable();
        }
        std::vector<std::unique_ptr<Operand>> operands;
        operands.push_back(makeOperand(I.dst()));
        operands.push_back(makeOperand(I.src1()));
        operands.push_back(makeOperand(I.src2()));
        builder_.add(std::make_unique<RISCVInstruction>(opcode, std::move(operands)));
    }

    static std::unique_ptr<LabelOperand> makeOperand(const mir::BasicBlockOperand &op) {
        return std::make_unique<LabelOperand>(op->name());
    }

    static std::unique_ptr<LabelOperand> makeOperand(const mir::GlobalVarOperand &op) {
        return std::make_unique<LabelOperand>(op->name());
    }

    static std::unique_ptr<LabelOperand> makeOperand(const mir::FunctionOperand &op) {
        return std::make_unique<LabelOperand>(op->name());
    }

    static std::unique_ptr<ImmediateOperand> makeOperand(const mir::ImmediateOperand &op) {
        return std::make_unique<ImmediateOperand>(op->value());
    }

    static std::unique_ptr<RISCVRegisterOperand> makeOperand(const mir::RegisterOperand &op) {
        return std::make_unique<RISCVRegisterOperand>(static_cast<const mir::RISCVRegister *>(&*op)->idx());
    }

    static std::unique_ptr<RISCVMemoryOperand> makeOperand(const mir::MemoryOperand &op) {
        return std::make_unique<RISCVMemoryOperand>(static_cast<const mir::RISCVRegister *>(&*op.baseReg())->idx(),
                                                    op.offset()->value());
    }
};

} // namespace

void RISCVMCGen::emit() {
    for (const mir::GlobalVar &G : MM_->globalVars) {
        if (G.hasInitializer()) {
            Section section;
            if (dynamic_cast<const mir::ZeroConstant *>(&G.initializer())) {
                section = Section::kBSS;
            } else {
                section = Section::kData;
            }

            bool isGlobal;
            if (G.linkage() == Linkage::kExternal) {
                isGlobal = true;
            } else {
                isGlobal = false;
            }

            std::string name = G.name();

            Fragment fragment(section, isGlobal, std::move(name));
            emitGlobalVar(G, fragment);

            program_->append(std::move(fragment));
        }
    }

    for (const mir::Function &F : MM_->functions) {
        if (!F.empty()) {
            Section section = Section::kText;

            bool isGlobal;
            if (F.linkage() == Linkage::kExternal) {
                isGlobal = true;
            } else {
                isGlobal = false;
            }

            std::string name = F.name();

            Fragment fragment(section, isGlobal, std::move(name));
            emitFunction(F, fragment);

            program_->append(std::move(fragment));
        }
    }
}

void RISCVMCGen::emitGlobalVar(const mir::GlobalVar &G, Fragment &fragment) {
    ConstantVisitorImpl visitor;
    visitor.builder().setPos(&fragment);
    G.initializer().accept(visitor);
}

void RISCVMCGen::emitFunction(const mir::Function &F, Fragment &fragment) {
    RISCVInstructionVisitorImpl visitor;
    visitor.builder().setPos(&fragment);
#ifndef NDEBUG
    for (const mir::BasicBlock &B : F) {
        for (const mir::Instruction &I : B) {
            for (const mir::RegisterOperand *op : I.regOps()) {
                assert(dynamic_cast<const mir::PhysicalRegister *>(&**op));
            }
        }
    }
#endif
    for (const mir::BasicBlock &B : F) {
        fragment.append(std::make_unique<Label>(B.name()));
        for (const mir::Instruction &I : B) {
            I.accept(visitor);
        }
    }
}
