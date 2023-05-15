#include "../ast/BinaryExprAST.h"
#include "../ast/ExprAST.h"
#include "../ast/NumberExprAST.h"
#include "../ast/VariableExprAST.h"
#include "../utils/Logger.h"
#include "Parser.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

std::map<int, int> binary_op_precedence = {
    {tok_add, 20},
    {tok_sub, 20},
    {tok_mul, 40},
    {tok_div, 40},
    {tok_and, 120},
    {tok_or, 130},
    {tok_lt, 70},
    {tok_le, 70},
    {tok_gt, 70},
    {tok_ge, 70},
    {tok_eq, 80},
    {tok_ne, 80},
};

static int get_token_precedence() {
    if (binary_op_precedence.find(current_token) == binary_op_precedence.end()) {
        return -1;
    }

    return binary_op_precedence[current_token];
}

std::unique_ptr<ExprAST> parse_expression() {
    auto lhs = parse_primary();
    if (!lhs) {
        return nullptr;
    }
    return parse_binary_op_rhs(0, std::move(lhs));
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
    switch (current_token) {
        case tok_identifier:
            return parse_identifier_expr();
        case tok_integer_number:
            return parse_integer_number_expr();
        case tok_float_number:
            return parse_float_number_expr();
        case '(':
            return parse_parenthesis_expr();
        default:
            std::string error_message = "Unknown token: " + std::to_string(current_token);
            return log_error(error_message);
    }
}

std::unique_ptr<ExprAST> parse_binary_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs) {
    while (true) {
        int token_prec = get_token_precedence();
        if (token_prec < expr_prec) {
            return lhs;
        }

        int bin_op = current_token;
        get_next_token();

        auto rhs = parse_primary();
        if (!rhs) {
            return nullptr;
        }

        int next_prec = get_token_precedence();
        if (token_prec < next_prec) {
            rhs = parse_binary_op_rhs(token_prec + 1, std::move(rhs));
            if (!rhs) {
                return nullptr;
            }
        }

        lhs = std::make_unique<BinaryExprAST>("+", bin_op, std::move(lhs), std::move(rhs));  // todo: 支持多种binop的字符串表示，需要实现token2str用于返回表示binop的字符串 
    }
}

std::unique_ptr<FunctionAST> parse_top_level_expression() {
    if (auto expr = parse_expression()) {
        llvm::Type *return_type = llvm::Type::getFloatTy(*context);  // 默认返回类型为 float

        auto proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::pair<std::string, llvm::Type *>>(), return_type);
        return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    }
    return nullptr;
}