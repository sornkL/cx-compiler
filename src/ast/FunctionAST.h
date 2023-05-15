#include "ExprAST.h"
#include "PrototypeAST.h"
#include "../cx/cx.h"

class FunctionAST {
    std::unique_ptr<PrototypeAST> proto;
    std::unique_ptr<ExprAST> body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
        : proto(std::move(proto)), body(std::move(body)) {}

    llvm::Function *codegen();
};