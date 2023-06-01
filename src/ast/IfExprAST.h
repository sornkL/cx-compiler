#include "ExprAST.h"
#include "../cx/CX.h"
#include "../utils/Logger.h"
#include "BlockAST.h"

class IfExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<BlockAST> then_body, else_body;

public:
    IfExprAST(std::unique_ptr<ExprAST> condition,
              std::unique_ptr<BlockAST> then_body,
              std::unique_ptr<BlockAST> else_body)
        : condition(std::move(condition)), then_body(std::move(then_body)), else_body(std::move(else_body)) {}
    
    llvm::Value *codegen() override;
};