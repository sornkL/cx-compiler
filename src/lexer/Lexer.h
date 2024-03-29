#ifndef __LEXER_H__
#define __LEXER_H__
#include "Token.h"
#include "../utils/CLI.h"
#include <string>

extern int current_token;  // 当前的token
extern bool skip_get_next_token;  // 是否要跳过读取下一个字符

extern std::string identifier;  // 标识符
extern int integer_number;  // 整数
extern float float_number;  // 浮点数
extern bool boolean;  // 布尔值

extern int lineno;  // 当前行号


int get_character();

void unget_character(int c);

int get_token();

int get_next_token();

#endif