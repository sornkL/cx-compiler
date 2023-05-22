#include "VariableExprAST.h"
#include <iostream>

llvm::Value *VariableExprAST::codegen() {

    // llvm::AllocaInst *alloca = named_values[name];
    
    // if (!alloca) {
    //     log_error("该变量未声明");
    // }

    // llvm::Type *alloca_type = alloca->getAllocatedType();

    // return builder->CreateLoad(alloca_type, alloca, name.c_str());

    return named_values[name];
}

std::string VariableExprAST::get_name() const {
    return name;
}