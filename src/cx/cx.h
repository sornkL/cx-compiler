#ifndef __CX_H__
#define __CX_H__

#include "../include/CXJIT.h"
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
extern std::unique_ptr<llvm::Module> modules;
extern std::unique_ptr<llvm::IRBuilder<>> builder;
extern std::map<std::string, llvm::AllocaInst *> named_values;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;
extern std::unique_ptr<llvm::orc::CXJIT> jit;

llvm::AllocaInst *create_entry_block_alloca(llvm::Function *function, 
                                            const std::string &var_name,
                                            llvm::Type *type);

bool is_value_number(llvm::Value *value);

bool is_value_float(llvm::Value *value);

bool is_value_integer(llvm::Value *value);

bool is_value_boolean(llvm::Value *value);

#endif