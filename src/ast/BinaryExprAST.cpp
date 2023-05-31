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

        llvm::AllocaInst *alloca = named_values[lhse->get_name()];
        llvm::Value *variable = builder->CreateLoad(alloca->getAllocatedType(), alloca, lhse->get_name());
        if (!variable) {
            return log_error_v("变量未定义");
        }

        if (variable->getType()->getTypeID() != value->getType()->getTypeID()) {
            return log_error_v("变量类型与声明不符");
        }
        builder->CreateStore(value, variable);
        return value;
    }

    auto lhs_value = lhs->codegen();
    auto rhs_value = rhs->codegen();

    if (!lhs_value || !rhs_value) {
        return nullptr;
    }
    
    if (is_value_boolean(lhs_value) && is_value_boolean(rhs_value)) {
        switch (op_token) {
            case tok_and:
                return builder->CreateAnd(lhs_value, rhs_value, "andtmp");
            case tok_or:
                return builder->CreateOr(lhs_value, rhs_value, "ortmp");
            default:
                return log_error_v("该运算符不支持布尔类型");
        }
    }

    if (is_value_number(lhs_value) && is_value_number(rhs_value)) {
        switch (op_token) {
            case tok_add:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateAdd(lhs_value, rhs_value, "addtmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    return builder->CreateFAdd(lhs_value, rhs_value, "addtmp");
                }
                break;
            case tok_sub:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateSub(lhs_value, rhs_value, "subtmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    return builder->CreateFSub(lhs_value, rhs_value, "subtmp");
                }
                break;
            case tok_mul:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateMul(lhs_value, rhs_value, "multmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    return builder->CreateFMul(lhs_value, rhs_value, "multmp");
                }
                break;
            case tok_div:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateSDiv(lhs_value, rhs_value, "divtmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    return builder->CreateFDiv(lhs_value, rhs_value, "divtmp");
                }
                break;
            case tok_lt:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateICmpSLT(lhs_value, rhs_value, "lttmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    auto res = builder->CreateFCmpULT(lhs_value, rhs_value, "lttmp");
                    return builder->CreateFPToUI(res, llvm::Type::getInt1Ty(*context), "lttmp");
                }
                break;
            case tok_le:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateICmpSLE(lhs_value, rhs_value, "letmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    auto res = builder->CreateFCmpULE(lhs_value, rhs_value, "letmp");
                    return builder->CreateFPToUI(res, llvm::Type::getInt1Ty(*context), "letmp");
                }
                break;
            case tok_gt:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateICmpSGT(lhs_value, rhs_value, "gttmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    auto res = builder->CreateFCmpUGT(lhs_value, rhs_value, "gttmp");
                    return builder->CreateFPToUI(res, llvm::Type::getInt1Ty(*context), "gttmp");
                }
                break;
            case tok_ge:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateICmpSGE(lhs_value, rhs_value, "getmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    auto res = builder->CreateFCmpUGE(lhs_value, rhs_value, "getmp");
                    return builder->CreateFPToUI(res, llvm::Type::getInt1Ty(*context), "getmp");
                }
                break;
            case tok_eq:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateICmpEQ(lhs_value, rhs_value, "eqtmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    auto res = builder->CreateFCmpUEQ(lhs_value, rhs_value, "eqtmp");
                    return builder->CreateFPToUI(res, llvm::Type::getInt1Ty(*context), "eqtmp");
                }
                break;
            case tok_ne:
                if (is_value_integer(lhs_value) && is_value_integer(rhs_value)) {
                    return builder->CreateICmpNE(lhs_value, rhs_value, "netmp");
                }
                if (is_value_float(lhs_value) && is_value_float(rhs_value)) {
                    auto res = builder->CreateFCmpUNE(lhs_value, rhs_value, "netmp");
                    return builder->CreateFPToUI(res, llvm::Type::getInt1Ty(*context), "netmp");
                }
                break;
            default:
                return log_error_v("该运算符不支持数字类型");
        }
    }
    return log_error_v("运算符左右两边类型不同");
}