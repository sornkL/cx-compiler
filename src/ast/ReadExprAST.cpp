#include "ReadExprAST.h"

llvm::Value *ReadExprAST::codegen() {
    llvm::AllocaInst *alloca = named_values[name];
    
    if (!alloca) {
        log_error("该变量未声明");
    }

    llvm::FunctionType *scanf_type = llvm::FunctionType::get(
        builder->getVoidTy(),
        {builder->getInt8Ty()->getPointerTo()},
        true
    );

    auto scanf_function = modules->getOrInsertFunction("scanf", scanf_type);

    llvm::Value *format_string = nullptr;
    llvm::Value *arg = nullptr;
    if (alloca->getAllocatedType()->isIntegerTy()) {
        format_string = builder->CreateGlobalStringPtr("%d");
    } else {
        format_string = builder->CreateGlobalStringPtr("%f");
    }

    builder->CreateCall(scanf_function, {format_string, alloca});

    return alloca;
}