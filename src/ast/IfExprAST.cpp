#include "IfExprAST.h"

llvm::Value *IfExprAST::codegen() {
    llvm::Value *cond = condition->codegen();
    if (!cond || !cond->getType()->isIntegerTy(1)) {
        return log_error_v("if语句的条件表达式必须是布尔类型");
    }

    llvm::Function *function = builder->GetInsertBlock()->getParent();

    llvm::BasicBlock *then_block = llvm::BasicBlock::Create(*context, "then", function);
    llvm::BasicBlock *else_block = llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock *merge_block = llvm::BasicBlock::Create(*context, "ifcont");

    builder->CreateCondBr(cond, then_block, else_block);

    builder->SetInsertPoint(then_block);

    llvm::Value *then_value = then_body->codegen();
    if (!then_value) {
        return nullptr;
    }

    builder->CreateBr(merge_block);
    then_block = builder->GetInsertBlock();

    function->insert(function->end(), else_block);
    builder->SetInsertPoint(else_block);

    if (else_body == nullptr) {
        return then_value;
    }

    llvm::Value *else_value = else_body->codegen();
    if (!else_value) {
        return nullptr;
    }

    builder->CreateBr(merge_block);
    else_block = builder->GetInsertBlock();

    function->insert(function->end(), merge_block);
    builder->SetInsertPoint(merge_block);

    llvm::PHINode *phi_node = builder->CreatePHI(function->getReturnType(), 2, "iftmp");

    phi_node->addIncoming(then_value, then_block);
    phi_node->addIncoming(else_value, else_block);
    return phi_node;
}