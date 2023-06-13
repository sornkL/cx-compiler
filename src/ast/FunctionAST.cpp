#include "ExprAST.h"
#include "FunctionAST.h"

llvm::Function *FunctionAST::codegen() {
    auto &proto_copy = *proto;
    function_protos[proto->get_callee()] = std::move(proto);
    llvm::Function *function = get_function(proto_copy.get_callee());

    if (!function) {
        return nullptr;
    }

    llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(basic_block);

    named_values.clear();
    for (auto &arg : function->args()) {
        llvm::AllocaInst *alloca = create_entry_block_alloca(function, arg.getName().str(), arg.getType());
        builder->CreateStore(&arg, alloca);
        named_values[arg.getName().str()] = alloca;
    }

    if (llvm::Value *ret = body->codegen()) {
        if (function->getReturnType()->isVoidTy()) {
            builder->CreateRetVoid();
        }
        llvm::verifyFunction(*function);
        if (is_optimize) {
            fpm->run(*function);
        }
        return function;
    }

    function->eraseFromParent();
    return nullptr;
}