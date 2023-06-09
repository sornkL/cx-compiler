#include "ExprAST.h"
#include "../utils/Logger.h"

class ReturnExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> expr;

public:
    ReturnExprAST(std::unique_ptr<ExprAST> expr) : expr(std::move(expr)) {}

    llvm::Value *codegen() override;
};