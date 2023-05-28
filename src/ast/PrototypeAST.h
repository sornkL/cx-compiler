#include "ExprAST.h"
#ifndef __PROTOTYPE_AST_H__
#define __PROTOTYPE_AST_H__
#include "../cx/CX.h"
#include <llvm/IR/Type.h>
#include <string>
#include <vector>

class PrototypeAST {
    std::string callee;
    std::vector<std::pair<std::string, llvm::Type *>> args;
    llvm::Type *return_type;

public:
    PrototypeAST(std::string callee, std::vector<std::pair<std::string, llvm::Type *>> args, llvm::Type *return_type)
        : callee(callee), args(std::move(args)), return_type(std::move(return_type)) {}

    llvm::Function *codegen();

    std::string get_callee() const;

    llvm::Type *get_return_type() const;

    void set_return_type(llvm::Type *return_type);
};

extern std::map<std::string, std::unique_ptr<PrototypeAST>> function_protos;

#endif