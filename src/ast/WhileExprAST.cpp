#include "WhileExprAST.h"

llvm::Value *WhileExprAST::codegen() {
    llvm::Value *cond = condition->codegen();
    if (!cond || !cond->getType()->isIntegerTy(1)) {
        return log_error_v("while语句的条件表达式必须是布尔类型");
    }

    llvm::Function *function = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *condition_block = llvm::BasicBlock::Create(*context, "whilecond", function);
    llvm::BasicBlock *body_block = llvm::BasicBlock::Create(*context, "whilebody", function);
    llvm::BasicBlock *end_block = llvm::BasicBlock::Create(*context, "whileend", function);

    builder->CreateBr(condition_block);
    builder->SetInsertPoint(condition_block);

    builder->CreateCondBr(cond, body_block, end_block);
    builder->SetInsertPoint(body_block);

    llvm::Value *body_value = while_body->codegen();
    if (!body_value) {
        return nullptr;
    }

    cond = condition->codegen();
    builder->CreateCondBr(cond, body_block, end_block);
    builder->SetInsertPoint(end_block);

    return body_value;
}