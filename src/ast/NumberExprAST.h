#include "ExprAST.h"
#include <optional>

class NumberExprAST : public ExprAST {
private:
    std::optional<int> integer_value;
    std::optional<double> float_value;

public:
    NumberExprAST(std::optional<int> integer_value, std::optional<double>) 
        : integer_value(integer_value), float_value(float_value) {}

    llvm::Value *codegen() override;
};

// temp
llvm::Value *NumberExprAST::codegen() {
    return nullptr;
}