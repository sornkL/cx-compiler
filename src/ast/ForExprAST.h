#include "ExprAST.h"
#include "BlockAST.h"
#include "../cx/CX.h"
#include "../utils/Logger.h"

class ForExprAST : public ExprAST {
private:
    std::unique_ptr<ExprAST> init, condition, increment;
    std::unique_ptr<BlockAST> for_body;

public:
    ForExprAST(std::unique_ptr<ExprAST> init,
               std::unique_ptr<ExprAST> condition,
               std::unique_ptr<ExprAST> increment,
               std::unique_ptr<BlockAST> for_body)
        : init(std::move(init)), condition(std::move(condition)),
          increment(std::move(increment)), for_body(std::move(for_body)) {}
    
    llvm::Value *codegen() override;
};