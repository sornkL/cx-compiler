#include "../ast/NumberExprAST.h"
#include "../ast/VariableExprAST.h"
#include "../utils/Logger.h"
#include "Parser.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

int current_token;

int get_next_token() {
    return current_token = get_token();
}

int get_token_precedence() {
    if (binary_op_precedence.find(current_token) == binary_op_precedence.end()) {
        return -1;
    }

    return binary_op_precedence[current_token];
}

std::unique_ptr<ExprAST> parse_expression() {
    return log_error("Not Implemented");
}

std::unique_ptr<ExprAST> parse_integer_number_expr() {
    auto result = std::make_unique<NumberExprAST>(integer_number, std::nullopt);
    get_next_token();
    return std::move(result);
}

std::unique_ptr<ExprAST> parse_float_number_expr() {
    auto result = std::make_unique<NumberExprAST>(std::nullopt, float_number);
    get_next_token();
    return std::move(result);
}

std::unique_ptr<ExprAST> parse_parenthesis_expr() {
    get_next_token();
    auto v = parse_expression();
    if (!v) {
        return nullptr;
    }

    if (current_token != ')') {
        return log_error("缺失 ')'");
    }
    get_next_token();
    return v;
}

std::unique_ptr<ExprAST> parse_identifier_expr() {
    return log_error("Not Implemented");
}

std::unique_ptr<ExprAST> parse_if_expression() {
    return log_error("Not Implemented");
}

std::unique_ptr<ExprAST> parse_while_expression() {
    return log_error("Not Implemented");
}

std::unique_ptr<ExprAST> parse_primary() {
    return log_error("Not Implementd");
}

std::unique_ptr<ExprAST> parse_binary_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs) {
    return log_error("Not Implemented");
}