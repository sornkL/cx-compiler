#include "NumberExprAST.h"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <iostream>

llvm::Value *NumberExprAST::codegen() {
    if (integer_value.has_value()) {
        return llvm::ConstantInt::get(*context, llvm::APInt(32, *integer_value));
    } else if (float_value.has_value()) {
        return llvm::ConstantFP::get(*context, llvm::APFloat(*float_value));
    } else {
        return nullptr;
    }
}