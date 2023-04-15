#include "ExprAST.h"

class IfExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> condition, then_body, else_body;

public:
    IfExprAST(std::unique_ptr<ExprAST> condition,
              std::unique_ptr<ExprAST> then_body,
              std::unique_ptr<ExprAST> else_body)
        : condition(std::move(condition)), then_body(std::move(then_body)), else_body(std::move(else_body)) {}
    
    llvm::Value *codegen() override;
};