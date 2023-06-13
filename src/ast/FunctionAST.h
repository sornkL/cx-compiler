#include "ExprAST.h"
#include "BlockAST.h"
#include "PrototypeAST.h"
#include "../cx/CX.h"
#include "../utils/CLI.h"

class FunctionAST {
private:
    std::unique_ptr<PrototypeAST> proto;
    std::unique_ptr<BlockAST> body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<BlockAST> body)
        : proto(std::move(proto)), body(std::move(body)) {}

    llvm::Function *codegen();
};