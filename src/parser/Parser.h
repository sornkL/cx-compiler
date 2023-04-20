#include "../ast/ExprAST.h"
#include "../lexer/Lexer.h"
#include <map>
#include <string>

// 二元运算符优先级，数值越高表示优先级越高
static std::map<int, int> binary_op_precedence = {
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

extern int current_token;

int get_next_token();

int get_token_precedence();

std::unique_ptr<ExprAST> parse_expression();

std::unique_ptr<ExprAST> parse_integer_number_expr();

std::unique_ptr<ExprAST> parse_float_number_expr(); 

std::unique_ptr<ExprAST> parse_parenthesis_expr();

std::unique_ptr<ExprAST> parse_identifier_expr();

std::unique_ptr<ExprAST> parse_if_expression();

std::unique_ptr<ExprAST> parse_while_expression();

std::unique_ptr<ExprAST> parse_primary();

std::unique_ptr<ExprAST> parse_binary_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs);