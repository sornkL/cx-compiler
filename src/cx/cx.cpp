#include "CX.h"

std::unique_ptr<llvm::LLVMContext> context;

std::unique_ptr<llvm::Module> modules;

std::unique_ptr<llvm::IRBuilder<>> builder;

std::map<std::string, llvm::AllocaInst *> named_values;

std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;

llvm::AllocaInst *create_entry_block_alloca(llvm::Function *function, 
                                            const std::string &var_name,
                                            llvm::Type *type) {
    llvm::IRBuilder<> temp(&function->getEntryBlock(), function->getEntryBlock().begin());

    return temp.CreateAlloca(type, nullptr, var_name);
}

bool is_value_number(llvm::Value *value) {
    return (!is_value_boolean(value)) && (value->getType()->isIntegerTy() || value->getType()->isFloatTy());
}

bool is_value_float(llvm::Value *value) {
    return value->getType()->isFloatTy() || value->getType()->isDoubleTy();
}

bool is_value_integer(llvm::Value *value) {
    return !is_value_boolean(value) && value->getType()->isIntegerTy();
}

bool is_value_boolean(llvm::Value *value) {
    return value->getType()->isIntegerTy(1);
}