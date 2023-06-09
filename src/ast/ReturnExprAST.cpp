#include "ReturnExprAST.h"

llvm::Value *ReturnExprAST::codegen() {
    llvm::Value *expr_value = expr->codegen();

    if (!expr_value) {
        return nullptr;
    }

    llvm::Function *function = builder->GetInsertBlock()->getParent();
    if (function->getReturnType()->isVoidTy()) {
        return log_error_v("没有返回值的函数不支持return");
    }

    if (function->getReturnType()->getTypeID() != expr_value->getType()->getTypeID()) {
        return log_error_v("返回值类型不匹配");
    }
    builder->CreateRet(expr_value);
    return expr_value;
}