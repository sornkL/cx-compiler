#include "BinaryExprAST.h"
#include "../lexer/Token.h"

llvm::Value *BinaryExprAST::codegen() {
    auto lhs_value = lhs->codegen();
    auto rhs_value = rhs->codegen();

    if (!lhs_value || !rhs_value) {
        return nullptr;
    }

    switch (op_token) {
        case tok_add:
            return builder->CreateFAdd(lhs_value, rhs_value, "addtmp");
        case tok_sub:
            return builder->CreateFSub(lhs_value, rhs_value, "subtmp");
        case tok_mul:
            return builder->CreateFMul(lhs_value, rhs_value, "multmp");
        case tok_div:
            return builder->CreateFDiv(lhs_value, rhs_value, "divtmp");
        case tok_lt:
            lhs_value = builder->CreateFCmpULT(lhs_value, rhs_value, "cmptmp");
            return builder->CreateUIToFP(lhs_value, llvm::Type::getDoubleTy(*context), "booltmp");
        default:
            return nullptr;
    }
}