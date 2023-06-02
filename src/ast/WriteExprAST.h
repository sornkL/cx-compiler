#include "ExprAST.h"
#include "../cx/CX.h"
#include <cstdio>

class WriteExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> expr;

public:
    WriteExprAST(std::unique_ptr<ExprAST> expr) : expr(std::move(expr)) {}

    llvm::Value *codegen() override;
};