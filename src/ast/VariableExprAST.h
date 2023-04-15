#include "ExprAST.h"
#include <string>

class VariableExprAST: public ExprAST {
private:
    std::string name;

public:
    VariableExprAST(const std::string &name) : name(name) {}

    llvm::Value *codegen() override;
};