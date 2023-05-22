#ifndef __VARIABLE_EXPR_AST_H__
#define __VARIABLE_EXPR_AST_H__
#include "ExprAST.h"
#include "../cx/CX.h"
#include "../utils/Logger.h"
#include <string>

class VariableExprAST: public ExprAST {
private:
    std::string name;

public:
    VariableExprAST(const std::string &name) : name(name) {}

    std::string get_name() const;

    llvm::Value *codegen() override;
};

#endif