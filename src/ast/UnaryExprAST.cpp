#include "UnaryExprAST.h"

llvm::Value *UnaryExprAST::codegen() {
    llvm::Value *operand_value = operand->codegen();
    if (!operand_value) {
        return nullptr;
    }

    if (is_value_boolean(operand_value)) {
        switch (op_token) {
            case tok_not:
                return builder->CreateNot(operand_value, "nottmp");
            default:
                return log_error_v("该运算符不支持布尔类型");
        }
    }
    return log_error_v("该运算符不支持该类型");
}