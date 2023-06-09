#include "ExprAST.h"
#include "../utils/Logger.h"
#include "../lexer/Token.h"
#include "../cx/CX.h"

class UnaryExprAST : public ExprAST {
private:
    int op_token;
    std::unique_ptr<ExprAST> operand;

public:
    UnaryExprAST(int op_token, std::unique_ptr<ExprAST> operand)
        : op_token(op_token), operand(std::move(operand)) {}
    
    llvm::Value *codegen() override;
};