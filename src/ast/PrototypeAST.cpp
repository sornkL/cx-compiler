#include "ExprAST.h"
#include "PrototypeAST.h"

llvm::Function *PrototypeAST::codegen() {
    std::vector<llvm::Type *> args_types;
    for (const auto &arg : args) {
        args_types.push_back(arg.second);
    }

    llvm::FunctionType *function_type = llvm::FunctionType::get(return_type, args_types, false);
    llvm::Function *function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, callee, *module);

    unsigned int index = 0;
    for (auto &arg : function->args()) {
        arg.setName(args[index++].first);
    }

    return function;
}

std::string PrototypeAST::getCallee() const {
    return callee;
}