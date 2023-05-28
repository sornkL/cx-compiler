#ifndef __BLOCK_AST_H__
#define __BLOCK_AST_H__
#include "ExprAST.h"
#include <vector>

class BlockAST {
private:
    std::vector<std::unique_ptr<ExprAST>> exprs;

public:
    BlockAST(std::vector<std::unique_ptr<ExprAST>> exprs)
        : exprs(std::move(exprs)) {}

    llvm::Value *codegen();

    void add_expr(std::unique_ptr<ExprAST> expr);
};

#endif