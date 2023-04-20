#include "Logger.h"
#include <iostream>

std::unique_ptr<ExprAST> log_error(const char *str) {
    std::cerr << "Error: " << str << std::endl;
    return nullptr;
}