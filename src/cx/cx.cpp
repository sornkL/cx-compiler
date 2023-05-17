#include "CX.h"

std::unique_ptr<llvm::LLVMContext> context;

std::unique_ptr<llvm::Module> module;

std::unique_ptr<llvm::IRBuilder<>> builder;

std::map<std::string, llvm::Value *> named_values;

std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;
