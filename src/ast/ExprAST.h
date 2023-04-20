#ifndef __EXPR_AST_H__
#define __EXPR_AST_H__
#include <llvm/IR/Value.h>

class ExprAST {
public:
    virtual ~ExprAST() = default;

    virtual llvm::Value *codegen() = 0;
};

#endif