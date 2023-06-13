#include "Logger.h"
#include <iostream> 

std::unique_ptr<ExprAST> log_error(const char *str) {
    std::cerr << "Error: line " << lineno << " " << str << std::endl;
    return nullptr;
}

std::unique_ptr<ExprAST> log_error(std::string str) {
    std::cerr << "Error: line " << lineno << " " << str << std::endl;
    return nullptr;
}

llvm::Value *log_error_v(const char *str) {
    std::cerr << "Error: line " << lineno << " " << str << std::endl;
    return nullptr;
}

std::unique_ptr<PrototypeAST> log_error_p(const char *str) {
    std::cerr << "Error: line " << lineno << " " << str << std::endl;
    return nullptr;
}

std::unique_ptr<BlockAST> log_error_b(const char *str) {
    std::cerr << "Error: line " << lineno << " " << str << std::endl;
    return nullptr;
}