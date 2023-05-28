#include "ExprAST.h"
#include "PrototypeAST.h"

std::map<std::string, std::unique_ptr<PrototypeAST>> function_protos;

llvm::Function *PrototypeAST::codegen() {
    std::vector<llvm::Type *> args_types;
    for (const auto &arg : args) {
        args_types.push_back(arg.second);
    }

    llvm::FunctionType *function_type = llvm::FunctionType::get(return_type, args_types, false);
    llvm::Function *function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, callee, *modules);

    unsigned int index = 0;
    for (auto &arg : function->args()) {
        arg.setName(args[index++].first);
    }

    return function;
}

std::string PrototypeAST::get_callee() const {
    return callee;
}

llvm::Type *PrototypeAST::get_return_type() const {
    return return_type;
}

void PrototypeAST::set_return_type(llvm::Type *return_type) {
    this->return_type = return_type;
}