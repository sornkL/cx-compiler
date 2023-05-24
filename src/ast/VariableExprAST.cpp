#include "VariableExprAST.h"
#include <iostream>

llvm::Value *VariableExprAST::codegen() {

    llvm::AllocaInst *alloca = named_values[name];
    
    if (!alloca) {
        log_error("该变量未声明");
    }

    return builder->CreateLoad(alloca->getAllocatedType(), alloca, name.c_str());
}

std::string VariableExprAST::get_name() const {
    return name;
}