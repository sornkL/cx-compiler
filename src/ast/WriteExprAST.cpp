#include "WriteExprAST.h"

llvm::Value *WriteExprAST::codegen() {
    llvm::Value *expr_value = expr->codegen();

    if (!expr_value) {
        return nullptr;
    }

    llvm::FunctionType *printf_type = llvm::FunctionType::get(
        builder->getInt32Ty(),
        {builder->getInt8Ty()->getPointerTo()},
        true
    );

    auto printf_function = modules->getOrInsertFunction("printf", printf_type);

    llvm::Value *format_string = nullptr;
    if (expr_value->getType()->isIntegerTy()) {
        format_string = builder->CreateGlobalStringPtr("%d");
    } else {
        format_string = builder->CreateGlobalStringPtr("%lf");
        expr_value = builder->CreateFPExt(expr_value, llvm::Type::getDoubleTy(*context));
    }

    builder->CreateCall(printf_function, {format_string, expr_value});
    builder->CreateCall(printf_function, builder->CreateGlobalStringPtr("\n"));

    return expr_value;
}