#include "BooleanExprAST.h"

llvm::Value *BooleanExprAST::codegen() {
    return llvm::ConstantInt::get(*context, llvm::APInt(1, boolean));
}