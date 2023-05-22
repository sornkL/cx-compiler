#include "ExprAST.h"
#include "FunctionAST.h"

llvm::Function *FunctionAST::codegen() {
    llvm::Function *function = modules->getFunction(proto->getCallee());

    if (!function) {
        function = proto->codegen();
    }

    if (!function) {
        return nullptr;
    }

    llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(basic_block);

    // named_values.clear();
    for (auto &arg : function->args()) {
        llvm::AllocaInst *alloca = create_entry_block_alloca(function, arg.getName().str(), arg.getType());
        builder->CreateStore(&arg, alloca);
        named_values[arg.getName().str()] = alloca;
    }

    if (llvm::Value *ret = body->codegen()) {
        builder->CreateRet(ret);
        llvm::verifyFunction(*function);
        // fpm->run(*function);
        return function;
    }

    function->eraseFromParent();
    return nullptr;
}