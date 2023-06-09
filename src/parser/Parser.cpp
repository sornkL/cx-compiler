#include "../ast/BinaryExprAST.h"
#include "../ast/BooleanExprAST.h"
#include "../ast/CallExprAST.h"
#include "../ast/ExprAST.h"
#include "../ast/NumberExprAST.h"
#include "../ast/VariableExprAST.h"
#include "../ast/DeclareExprAST.h"
#include "../ast/IfExprAST.h"
#include "../ast/WhileExprAST.h"
#include "../ast/WriteExprAST.h"
#include "../ast/ReadExprAST.h"
#include "../ast/ReturnExprAST.h"
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
    {tok_assign, 10},
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

std::unique_ptr<BlockAST> parse_block_expression() {
    std::vector<std::unique_ptr<ExprAST>> result;
    while (current_token != '}') {
        if (current_token == tok_eof) {
            return log_error_b("缺失 '}'");
        }
        auto expr = parse_expression();
        get_next_token();
        if (!expr) {
            return log_error_b("表达式错误");
        }
        result.push_back(std::move(expr));
    }
    return std::make_unique<BlockAST>(std::move(result));
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

std::unique_ptr<ExprAST> parse_boolean_expr() {
    auto result = std::make_unique<BooleanExprAST>(boolean);
    get_next_token();
    return std::move(result);
}

std::unique_ptr<ExprAST> parse_declaration_expr() {
    llvm::Type *id_type = nullptr;
    switch (current_token) {
        case tok_int:
            id_type = llvm::Type::getInt32Ty(*context);
            break;
        case tok_bool:
            id_type = llvm::Type::getInt1Ty(*context);
            break;
        case tok_float:
            id_type = llvm::Type::getFloatTy(*context);
            break;
        default:
            log_error("不支持该类型");
            break;
    }
    get_next_token();

    if (current_token != tok_identifier) {
        return log_error("缺失标识符");
    }

    std::string id_name = identifier;
    get_next_token();

    return std::make_unique<DeclareExprAST>(id_name, id_type);
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
    std::string id_name = identifier;

    get_next_token();
    if (current_token != '(') {
        return std::make_unique<VariableExprAST>(id_name);
    }

    get_next_token();
    std::vector<std::unique_ptr<ExprAST>> args;
    if (current_token != ')') {
        while (true) {
            if (auto arg = parse_expression()) {
                args.push_back(std::move(arg));
            } else {
                return nullptr;
            }

            if (current_token == ')') {
                break;
            }

            if (current_token != ',') {
                return log_error("缺失 ','");
            }
            get_next_token();
        }
    }

    get_next_token();
    return std::make_unique<CallExprAST>(id_name, std::move(args));
}

std::unique_ptr<ExprAST> parse_if_expression() {
    get_next_token();

    if (current_token != '(') {
        return log_error("缺失 '('");
    }

    get_next_token();
    auto condition = parse_expression();
    if (!condition) {
        return nullptr;
    }

    if (current_token != ')') {
        return log_error("缺失 ')'");
    }

    get_next_token();
    if (current_token != '{') {
        return log_error("缺失 '{'");
    }

    get_next_token();
    auto then_block = parse_block_expression();
    if (!then_block) {
        return nullptr;
    }

    if (current_token != '}') {
        return log_error("缺失 '}'");
    }

    get_next_token();
    if (current_token != tok_else) {
        skip_get_next_token = true;
        return std::make_unique<IfExprAST>(std::move(condition), std::move(then_block), nullptr);
    }

    get_next_token();
    if (current_token != '{') {
        return log_error("缺失 '{'");
    }

    get_next_token();
    auto else_block = parse_block_expression();
    if (!else_block) {
        return nullptr;
    }

    if (current_token != '}') {
        return log_error("缺失 '}'");
    }

    return std::make_unique<IfExprAST>(std::move(condition), std::move(then_block), std::move(else_block));
}

std::unique_ptr<ExprAST> parse_while_expression() {
    get_next_token();
    if (current_token != '(') {
        return log_error("缺失 '('");
    }

    get_next_token();
    auto condition = parse_expression();
    if (!condition) {
        return nullptr;
    }

    if (current_token != ')') {
        return log_error("缺失 ')'");
    }

    get_next_token();
    if (current_token != '{') {
        return log_error("缺失 '{'");
    }

    get_next_token();
    auto body = parse_block_expression();
    if (!body) {
        return nullptr;
    }

    if (current_token != '}') {
        return log_error("缺失 '}'");
    }

    return std::make_unique<WhileExprAST>(std::move(condition), std::move(body));
}

std::unique_ptr<ExprAST> parse_write_expression() {
    get_next_token();
    std::unique_ptr<ExprAST> expr = parse_expression();
    if (!expr) {
        return log_error("表达式错误");
    }
    return std::make_unique<WriteExprAST>(std::move(expr));
}

std::unique_ptr<ExprAST> parse_read_expression() {
    get_next_token();
    std::string id_name = identifier;
    get_next_token();
    return std::make_unique<ReadExprAST>(id_name);
}

std::unique_ptr<ExprAST> parse_return_expression() {
    get_next_token();
    std::unique_ptr<ExprAST> expr = parse_expression();
    if (!expr) {
        return log_error("表达式错误");
    }
    return std::make_unique<ReturnExprAST>(std::move(expr));
}

std::unique_ptr<PrototypeAST> parse_prototype() {
    std::string function_name = identifier;

    get_next_token();
    if (current_token != '(') {
        return log_error_p("缺失 '('");
    }

    std::vector<std::pair<std::string, llvm::Type *>> args;
    std::pair<std::string, llvm::Type *> temp_arg;
    std::string temp_arg_name;
    llvm::Type *temp_arg_type = nullptr;
    bool has_args = false;
    get_next_token();
    while (current_token == tok_int ||
           current_token == tok_bool || 
           current_token == tok_float || 
           current_token == ':' || 
           current_token == ',' || 
           current_token == tok_identifier) {
        switch (current_token) {
            case tok_int:
                temp_arg_type = llvm::Type::getInt32Ty(*context);
                break;
            case tok_bool:
                temp_arg_type = llvm::Type::getInt1Ty(*context);
                break;
            case tok_float:
                temp_arg_type = llvm::Type::getFloatTy(*context);
                break;
            case tok_identifier:
                temp_arg_name = identifier;
                break;
            case ':':
                break;
            case ',':
                temp_arg = std::make_pair(temp_arg_name, temp_arg_type);
                args.push_back(temp_arg);
                break;
            default:
                break;
        }
        has_args = true;
        get_next_token();
    }
    if (current_token != ')') {
        return log_error_p("缺失 ')'");
    }

    if (has_args) {
        temp_arg = std::make_pair(temp_arg_name, temp_arg_type);
        args.push_back(temp_arg);
    }
    get_next_token();
    
    llvm::Type *return_type = llvm::Type::getVoidTy(*context);
    if (current_token != tok_arrow) {
        return std::make_unique<PrototypeAST>(function_name, args, return_type);
    }
    get_next_token();
    switch (current_token) {
        case tok_int:
            return_type = llvm::Type::getInt32Ty(*context);
            break;
        case tok_bool:
            return_type = llvm::Type::getInt1Ty(*context);
            break;
        case tok_float:
            return_type = llvm::Type::getFloatTy(*context);
            break;
        default:
            return log_error_p("无返回类型或不支持该返回类型");
            break;
    }
    get_next_token();
    return std::make_unique<PrototypeAST>(function_name, args, return_type);
}

std::unique_ptr<ExprAST> parse_primary() {
    switch (current_token) {
        case tok_identifier:
            return parse_identifier_expr();
        case tok_integer_number:
            return parse_integer_number_expr();
        case tok_float_number:
            return parse_float_number_expr();
        case tok_true:
            return parse_boolean_expr();
        case tok_false:
            return parse_boolean_expr();
        case '(':
            return parse_parenthesis_expr();
        case tok_int:
            return parse_declaration_expr();
        case tok_bool:
            return parse_declaration_expr();
        case tok_float:
            return parse_declaration_expr();
        case tok_if:
            return parse_if_expression();
        case tok_while:
            return parse_while_expression();
        case tok_write:
            return parse_write_expression();
        case tok_read:
            return parse_read_expression();
        case tok_return:
            return parse_return_expression();
        default:
            std::string error_message = "未知的token: " + std::to_string(current_token);
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
    get_next_token();
    if (auto body = parse_block_expression()) {
        llvm::Type *return_type = llvm::Type::getVoidTy(*context);  // 默认返回类型为void

        auto proto = std::make_unique<PrototypeAST>("main", std::vector<std::pair<std::string, llvm::Type *>>(), return_type);
        return std::make_unique<FunctionAST>(std::move(proto), std::move(body));
    }
    return nullptr;
}

std::unique_ptr<FunctionAST> parse_function_definition() {
    get_next_token();
    auto proto = parse_prototype();
    if (!proto || current_token != '{') {
        return nullptr;
    }

    get_next_token();
    if (auto body = parse_block_expression()) {
        return std::make_unique<FunctionAST>(std::move(proto), std::move(body));
    }
    get_next_token();
    return nullptr;
}