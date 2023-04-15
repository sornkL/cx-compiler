#include "Token.h"
#include <string>

static std::string identifier;  // 标识符
static int integer_number;  // 整数
static int float_number;  // 浮点数


int get_character();

void unget_character(int c);

int get_token();