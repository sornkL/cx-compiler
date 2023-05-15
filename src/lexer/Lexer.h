#ifndef __LEXER_H__
#define __LEXER_H__
#include "Token.h"
#include <string>

extern int current_token;  // 当前的token

extern std::string identifier;  // 标识符
extern int integer_number;  // 整数
extern double float_number;  // 浮点数


int get_character();

void unget_character(int c);

int get_token();

int get_next_token();

#endif