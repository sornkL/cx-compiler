#include "DeclareExprAST.h"

llvm::Value *DeclareExprAST::codegen() {
    llvm::AllocaInst *old;

    old = named_values[name];
    if (old) {
        named_values[name] = old;
        return log_error_v("重复声明变量");
    }

    llvm::Function *function = builder->GetInsertBlock()->getParent();
    llvm::AllocaInst *alloca = create_entry_block_alloca(function, name, type);

    llvm::Value *default_value = nullptr;

    if (type->isIntegerTy(1)) {  // 布尔型初始化
        default_value = llvm::ConstantInt::get(type, false);
    } else if (type->isIntegerTy()) {  // 整型初始化
        default_value = llvm::ConstantInt::get(type, 0);
    } else if (type->isFloatTy()) {  // 浮点型初始化
        default_value = llvm::ConstantFP::get(type, 0.0);
    } else {
        return log_error_v("不支持该类型");
    }
    builder->CreateStore(default_value, alloca);

    named_values[name] = alloca;
    return alloca;
}