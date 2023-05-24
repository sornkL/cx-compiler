#include "ExprAST.h"
#include "../cx/CX.h"
#include <optional>

class NumberExprAST : public ExprAST {
private:
    std::optional<int> integer_value;
    std::optional<float> float_value;

public:
    NumberExprAST(std::optional<int> integer_value, std::optional<float> float_value) 
        : integer_value(integer_value), float_value(float_value) {}

    llvm::Value *codegen() override;
};