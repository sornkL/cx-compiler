#include "CallExprAST.h"

llvm::Value *CallExprAST::codegen() {
    llvm::Function *callee_function = get_function(callee);
    if (!callee_function) {
        return log_error_v("函数未定义");
    }

    if (callee_function->arg_size() != args.size()) {
        return log_error_v("函数参数数量不匹配");
    }

    std::vector<llvm::Value *> args_values;
    for (unsigned int i=0, e=args.size(); i!=e; ++i) {
        llvm::Value *arg_value = args[i]->codegen();
        if (arg_value->getType()->getTypeID() != callee_function->getArg(i)->getType()->getTypeID()) {
            return log_error_v("函数参数类型不匹配");
        }
        args_values.push_back(arg_value);
        if (!args_values.back()) {
            return nullptr;
        }
    }

    return builder->CreateCall(callee_function, args_values, "calltmp");
}