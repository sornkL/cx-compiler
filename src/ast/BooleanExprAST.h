#include "ExprAST.h"
#include "../cx/CX.h"

class BooleanExprAST : public ExprAST {
private:
    bool boolean;

public:
    BooleanExprAST(bool boolean) : boolean(boolean) {}

    llvm::Value *codegen() override;
};