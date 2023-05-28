#include "ExprAST.h"
#include "../utils/Logger.h"
#include "../cx/CX.h"
#include <llvm/IR/Type.h>
#include <string>
#include <vector>

class CallExprAST : public ExprAST {
private:
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;

public:
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
        : callee(callee), args(std::move(args)) {}
    
    llvm::Value *codegen() override;
};