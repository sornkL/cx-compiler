#include "ExprAST.h"
#include "../cx/CX.h"
#include "../utils/Logger.h"
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <string>

class DeclareExprAST : public ExprAST {
private:
    std::string name;
    llvm::Type *type;

public:
    DeclareExprAST(const std::string &name, llvm::Type *type) : name(name), type(type) {}

    llvm::Value *codegen() override;
};
