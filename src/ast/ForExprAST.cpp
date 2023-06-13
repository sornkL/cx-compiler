#include "ForExprAST.h"

llvm::Value *ForExprAST::codegen() {
    llvm::Value *init_value = init->codegen();
    if (!init_value) {
        return nullptr;
    }

    llvm::Value *cond = condition->codegen();
    if (!cond || !cond->getType()->isIntegerTy(1)) {
        return log_error_v("for语句的条件表达式必须是布尔类型");
    }

    llvm::Function *function = builder->GetInsertBlock()->getParent();


    llvm::BasicBlock *condition_block = llvm::BasicBlock::Create(*context, "forcond", function);
    llvm::BasicBlock *body_block = llvm::BasicBlock::Create(*context, "forbody", function);
    llvm::BasicBlock *end_block = llvm::BasicBlock::Create(*context, "forend", function);

    builder->CreateBr(condition_block);
    builder->SetInsertPoint(condition_block);

    builder->CreateCondBr(cond, body_block, end_block);
    builder->SetInsertPoint(body_block);

    llvm::Value *body_value = for_body->codegen();
    llvm::Value *increment_value = increment->codegen();
    if (!increment_value) {
        return nullptr;
    }
    
    cond = condition->codegen();
    builder->CreateCondBr(cond, body_block, end_block);
    builder->SetInsertPoint(end_block);

    return body_value;
}