#include "ExprAST.h"
#include <optional>

class NumberExprAST : public ExprAST {
private:
    int integer_value;
    std::optional<double> float_value;

public:
    NumberExprAST(int integer_value) : integer_value(integer_value) {}
    NumberExprAST(double float_value) : float_value(float_value) {}

    llvm::Value *codegen() override;
};