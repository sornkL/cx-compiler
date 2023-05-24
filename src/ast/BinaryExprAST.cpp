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

    // todo 支持浮点数
    if (op_token == tok_add) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("+ 左右两边必须是数字");
        }
        return builder->CreateAdd(lhs_value, rhs_value, "addtmp");
    } else if (op_token == tok_sub) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("- 左右两边必须是数字");
        }
        return builder->CreateSub(lhs_value, rhs_value, "subtmp");
    } else if (op_token == tok_mul) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("* 左右两边必须是数字");
        }
        return builder->CreateMul(lhs_value, rhs_value, "multmp");
    } else if (op_token == tok_div) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("/ 左右两边必须是数字");
        }
        return builder->CreateFDiv(lhs_value, rhs_value, "divtmp");
    } else if (op_token == tok_and) {
        if (!is_value_boolean(lhs_value) || !is_value_boolean(rhs_value)) {
            return log_error_v("&& 左右两边必须是布尔值");
        }
        return builder->CreateAnd(lhs_value, rhs_value, "andtmp");
    } else if (op_token == tok_or) {
        if (!is_value_boolean(lhs_value) || !is_value_boolean(rhs_value)) {
            return log_error_v("|| 左右两边必须是布尔值");
        }
        return builder->CreateOr(lhs_value, rhs_value, "ortmp");
    } else if (op_token == tok_lt) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("< 左右两边必须是数字");
        }
        lhs_value = builder->CreateICmpULE(lhs_value, rhs_value, "ulttmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getInt1Ty(*context), "booltmp");
    } else if (op_token == tok_le) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("<= 左右两边必须是数字");
        }
        lhs_value = builder->CreateICmpULE(lhs_value, rhs_value, "uletmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getInt1Ty(*context), "booltmp");
    } else if (op_token == tok_gt) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("> 左右两边必须是数字");
        }
        lhs_value = builder->CreateICmpUGT(lhs_value, rhs_value, "ugttmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getInt1Ty(*context), "booltmp");
    } else if (op_token == tok_ge) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v(">= 左右两边必须是数字");
        }
        lhs_value = builder->CreateICmpUGE(lhs_value, rhs_value, "ugetmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getInt1Ty(*context), "booltmp");
    } else if (op_token == tok_eq) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("== 左右两边必须是数字");
        }
        lhs_value = builder->CreateICmpEQ(lhs_value, rhs_value, "ueqtmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getInt1Ty(*context), "booltmp");
    } else if (op_token == tok_ne) {
        if (!is_value_number(lhs_value) || !is_value_number(rhs_value)) {
            return log_error_v("!= 左右两边必须是数字");
        }
        lhs_value = builder->CreateICmpNE(lhs_value, rhs_value, "unetmp");
        return builder->CreateUIToFP(lhs_value, llvm::Type::getInt1Ty(*context), "booltmp");
    } else {
        return nullptr;
    }
}