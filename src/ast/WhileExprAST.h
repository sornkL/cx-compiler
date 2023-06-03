#include "ExprAST.h"
#include "BlockAST.h"
#include "../cx/CX.h"
#include "../utils/Logger.h"

class WhileExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<BlockAST> while_body;

public:
    WhileExprAST(std::unique_ptr<ExprAST> condition,
                 std::unique_ptr<BlockAST> while_body)
        : condition(std::move(condition)), while_body(std::move(while_body)) {}
    
    llvm::Value *codegen() override;
};