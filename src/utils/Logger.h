#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "ast/ExprAST.h"
#include <memory>

std::unique_ptr<ExprAST> log_error(const char *str);
std::unique_ptr<ExprAST> log_error(std::string str);
llvm::Value *log_error_v(const char *str);

#endif