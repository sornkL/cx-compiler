#include "SwitchExprAST.h"

llvm::Value *SwitchExprAST::codegen() {
    llvm::Value *cond = condition->codegen();
    if (!cond || !cond->getType()->isIntegerTy(32)) {
        return log_error_v("switch语句的条件表达式必须是整型");
    }

    llvm::Function *function = builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *default_block = llvm::BasicBlock::Create(*context, "switchdefault", function);
    llvm::BasicBlock *end_block = llvm::BasicBlock::Create(*context, "switchend", function);

    llvm::SwitchInst *switch_inst = builder->CreateSwitch(cond, default_block, cases.size());

    bool has_return = true;

    for (auto &case_value_expr : cases) {
        llvm::BasicBlock *case_block = llvm::BasicBlock::Create(*context, "switchcase", function);
        builder->SetInsertPoint(case_block);

        llvm::Value *case_value = case_value_expr.first->codegen();
        if (!case_value) {
            return nullptr;
        }

        switch_inst->addCase(llvm::cast<llvm::ConstantInt>(case_value), case_block);

        case_value_expr.second->codegen();

        if ((case_block->getTerminator() && !case_block->getTerminator()->isTerminator()) || (!case_block->getTerminator())) {
            builder->CreateBr(end_block);
            has_return = false;
        }
    }
    
    builder->SetInsertPoint(default_block);
    llvm::Value *default_value = default_case->codegen();
    if ((default_block->getTerminator() && !default_block->getTerminator()->isTerminator()) || (!default_block->getTerminator())) {
            builder->CreateBr(end_block);
        }
    
    builder->SetInsertPoint(end_block);
    if (has_return) {
        // 当所有case分支与default分支都有return时，在合并代码块中添加返回语句保证完整性
        builder->CreateRetVoid();
    }

    return default_value;
}