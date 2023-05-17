#include "ExprAST.h"
#include "../cx/CX.h"

class BinaryExprAST : public ExprAST {
private:
    std::string op;
    int op_token;
    std::unique_ptr<ExprAST> lhs, rhs;

public:
    BinaryExprAST(std::string op,
                  int op_token,
                  std::unique_ptr<ExprAST> lhs, 
                  std::unique_ptr<ExprAST> rhs)
        : op(std::move(op)), op_token(std::move(op_token)), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    llvm::Value *codegen() override;
};