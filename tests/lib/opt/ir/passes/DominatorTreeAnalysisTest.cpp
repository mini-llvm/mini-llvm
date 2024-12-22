#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <gtest/gtest.h>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "TestUtils.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

TEST(DominatorTreeAnalysisTest, test0) {
    std::shared_ptr<Function> F = parseFunction(R"(
define void @foo(i1 %0) {
1:
    br i1 %0, label %2, label %5

2:
    br i1 %0, label %3, label %4

3:
    br label %7

4:
    br label %7

5:
    br i1 %0, label %6, label %7

6:
    br label %5

7:
    ret void
}
)");

    auto i = F->begin();
    const BasicBlock *B1 = &*i++, *B2 = &*i++, *B3 = &*i++, *B4 = &*i++, *B5 = &*i++, *B6 = &*i++, *B7 = &*i++;

    std::unordered_map<const BasicBlock *, std::unordered_set<const BasicBlock *>> dom{
        {B1, {B1, B2, B3, B4, B5, B6, B7}},
        {B2, {B2, B3, B4}},
        {B3, {B3}},
        {B4, {B4}},
        {B5, {B5, B6}},
        {B6, {B6}},
        {B7, {B7}}
    };

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(*F);

    for (const BasicBlock &u : *F) {
        for (const BasicBlock &v : *F) {
            if (dom[&u].contains(&v)) {
                EXPECT_TRUE(domTree.dominates(u, v));
            } else {
                EXPECT_FALSE(domTree.dominates(u, v));
            }
        }
    }
}
