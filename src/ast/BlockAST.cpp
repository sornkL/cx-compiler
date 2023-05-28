#include "BlockAST.h"

llvm::Value *BlockAST::codegen() {
    llvm::Value *last = nullptr;
    for (auto &expr : exprs) {
        last = expr->codegen();
    }
    return last;
}

void BlockAST::add_expr(std::unique_ptr<ExprAST> expr) {
    exprs.push_back(std::move(expr));
}