#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "ast/ExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/BlockAST.h"
#include "lexer/Lexer.h"
#include <memory>

std::unique_ptr<ExprAST> log_error(const char *str);
std::unique_ptr<ExprAST> log_error(std::string str);
llvm::Value *log_error_v(const char *str);
std::unique_ptr<PrototypeAST> log_error_p(const char *str);
std::unique_ptr<BlockAST> log_error_b(const char *str);

#endif