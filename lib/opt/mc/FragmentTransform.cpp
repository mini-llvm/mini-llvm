#include "mini-llvm/opt/mc/FragmentTransform.h"

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/Program.h"

using namespace mini_llvm::mc;

bool FragmentTransform::runOnProgram(Program &program) {
    bool changed = false;
    for (Fragment &fragment : program) {
        changed |= runOnFragment(fragment);
    }
    return changed;
}
