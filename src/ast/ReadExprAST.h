#include "ExprAST.h"
#include "../cx/CX.h"
#include "../utils/Logger.h"
#include <cstdio>
#include <string>

class ReadExprAST : public ExprAST {
private:
    std::string name;

public:
    ReadExprAST(const std::string &name) : name(name) {}

    llvm::Value *codegen() override;
};