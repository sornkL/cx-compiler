#ifndef __CX_H__
#define __CX_H__

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <map>

extern std::unique_ptr<llvm::LLVMContext> context;
extern std::unique_ptr<llvm::Module> module;
extern std::unique_ptr<llvm::IRBuilder<>> builder;
extern std::map<std::string, llvm::Value *> named_values;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;

#endif