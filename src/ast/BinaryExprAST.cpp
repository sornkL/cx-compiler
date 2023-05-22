#include "BinaryExprAST.h"
#include "VariableExprAST.h"
#include "../utils/Logger.h"
#include "../lexer/Token.h"

llvm::Value *BinaryExprAST::codegen() {
    if (op_token == tok_assign) {
        VariableExprAST *lhse = static_cast<VariableExprAST *>(lhs.get());
        if (!lhse) {
            return log_error_v("= 左边必须是变量");
        }
        
        llvm::Value *value = rhs->codegen();
        if (!value) {
            return nullptr;
        }
        llvm::Value *variable = named_values[lhse->get_name()];
        if (!variable) {
            return log_error_v("变量未定义");
        }
        builder->CreateStore(value, variable);
        return value;
    }

    auto lhs_value = lhs->codegen();
    auto rhs_value = rhs->codegen();

    if (!lhs_value || !rhs_value) {
        return nullptr;
    }

    if (op_token == tok_add) {
        return builder->CreateAdd(lhs_value, rhs_value, "addtmp");
    } else if (op_token == tok_sub) {
        return builder->CreateSub(lhs_value, rhs_value, "subtmp");
    } else if (op_token == tok_mul) {
        return builder->CreateMul(lhs_value, rhs_value, "multmp");
    } else if (op_token == tok_div) {
        return builder->CreateFDiv(lhs_value, rhs_value, "divtmp");
    } else if (op_token == tok_lt) {
        lhs_value = builder->CreateFCmpULT(lhs_value, rhs_value, "cmptmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getDoubleTy(*context), "booltmp");
    } else {
        return nullptr;
    }
}