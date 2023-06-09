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

    bool has_return = true;

    if (else_body) {
        builder->CreateCondBr(cond, then_block, else_block);
    } else {
        builder->CreateCondBr(cond, then_block, merge_block);
    }

    builder->SetInsertPoint(then_block);
    llvm::Value *then_value = then_body->codegen();
    if (!then_value) {
        return nullptr;
    }

    if ((then_block->getTerminator() && !then_block->getTerminator()->isTerminator()) || (!then_block->getTerminator())) {
        builder->CreateBr(merge_block);
        has_return = false;
    }
    then_block = builder->GetInsertBlock();

    llvm::Value *else_value = nullptr;
    if (else_body) {
        function->insert(function->end(), else_block);
        builder->SetInsertPoint(else_block);

        else_value = else_body->codegen();
        if (!else_value) {
            return nullptr;
        }

        if ((else_block->getTerminator() && !else_block->getTerminator()->isTerminator()) || (!else_block->getTerminator())) {
            builder->CreateBr(merge_block);
            has_return = false;
        }
        else_block = builder->GetInsertBlock();
    }

    function->insert(function->end(), merge_block);
    builder->SetInsertPoint(merge_block);

    unsigned int reversed_values = else_body ? 2 : 1;
    llvm::PHINode *phi_node = builder->CreatePHI(then_value->getType(), reversed_values, "iftmp");

    phi_node->addIncoming(then_value, then_block);
    if (else_body) {
        phi_node->addIncoming(else_value, else_block);
    }
    
    if (has_return && else_body) {
        // 当then和else分支都有return时，在合并代码块中添加返回语句保证完整性
        builder->CreateRetVoid();
    }

    return phi_node;
}