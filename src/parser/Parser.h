#include "../ast/ExprAST.h"
#include "../ast/FunctionAST.h"
#include "../lexer/Lexer.h"
#include <map>
#include <string>

// 二元运算符优先级，数值越高表示优先级越高
extern std::map<int, int> binary_op_precedence;

std::unique_ptr<ExprAST> parse_expression();

std::unique_ptr<ExprAST> parse_integer_number_expr();

std::unique_ptr<ExprAST> parse_float_number_expr(); 

std::unique_ptr<ExprAST> parse_declaration_expr();

std::unique_ptr<ExprAST> parse_parenthesis_expr();

std::unique_ptr<ExprAST> parse_identifier_expr();

std::unique_ptr<ExprAST> parse_if_expression();

std::unique_ptr<ExprAST> parse_while_expression();

std::unique_ptr<ExprAST> parse_primary();

std::unique_ptr<ExprAST> parse_binary_op_rhs(int expr_prec, std::unique_ptr<ExprAST> lhs);

std::unique_ptr<FunctionAST> parse_top_level_expression();
