#include "ast/ExprAST.h"
#include <memory>

std::unique_ptr<ExprAST> log_error(const char *str);
std::unique_ptr<ExprAST> log_error(std::string str);