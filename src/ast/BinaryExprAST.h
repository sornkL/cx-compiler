#include "ExprAST.h"

class BinaryExprAST : public ExprAST {
private:
    std::string op;
    std::unique_ptr<ExprAST> lhs, rhs;

public:
    BinaryExprAST(std::string op, 
                  std::unique_ptr<ExprAST> lhs, 
                  std::unique_ptr<ExprAST> rhs)
        : op(std::move(op)), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    llvm::Value *codegen() override;
};