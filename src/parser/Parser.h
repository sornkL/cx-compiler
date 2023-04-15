#include "ast/ExprAST.h"
#include "lexer/Lexer.h"
#include <map>
#include <string>

static std::map<std::string, int> binary_op_precedence;  // 二元运算符优先级

static int current_token;

int get_next_token();

int get_token_precedence(const std::string &binary_op);

std::unique_ptr<ExprAST> parse_expression();

std::unique_ptr<ExprAST> parse_number_expr();

std::unique_ptr<ExprAST> parse_parenthesis_expr();

std::unique_ptr<ExprAST> parse_identifier_expr();

std::unique_ptr<ExprAST> parse_if_expression();

std::unique_ptr<ExprAST> parse_while_expression();

std::unique_ptr<ExprAST> parse_primary();

std::unique_ptr<ExprAST> parse_binary_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs);