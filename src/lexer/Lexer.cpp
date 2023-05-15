#include "Lexer.h"
#include <iostream>

int current_token;

std::string identifier;
int integer_number;
double float_number;

int get_character() {
    return getchar();
}

void unget_character(int c) {
    ungetc(c, stdin);
}

int get_token() {
    static int last_char = ' ';

    while (isspace(last_char)) {
        last_char = get_character();
    }

    if (isalpha(last_char)) {  // [a-zA-Z][a-zA-Z0-9]*
        identifier = last_char;
        while (isalnum((last_char = get_character()))) {
            identifier += last_char;
        }

        if (identifier == "int") {
            return tok_int;
        }
        if (identifier == "bool") {
            return tok_bool;
        }
        if (identifier == "if") {
            return tok_if;
        }
        if (identifier == "else") {
            return tok_else;
        }
        if (identifier == "while") {
            return tok_while;
        }
        if (identifier == "write") {
            return tok_write;
        }
        if (identifier == "read") {
            return tok_read;
        }
        if (identifier == "true") {
            return tok_true;
        }
        if (identifier == "false") {
            return tok_false;
        }
        return tok_identifier;
    }

    if (isdigit(last_char) || last_char == '.') {  // [0-9.]+
        std::string num_str;
        bool is_float = false;

        do {
            if (last_char == '.') {
                is_float = true;
            }
            num_str += last_char;
            last_char = get_character();
        } while (isdigit(last_char) || last_char == '.');

        if (is_float) {
            float_number = std::stod(num_str);
            return tok_float_number;
        } else {
            integer_number = std::stoi(num_str);
            return tok_integer_number;
        }
    }

    if (last_char == '/') {  // 处理注释
        int next_char = get_character();
        if (next_char == '/') {  // 单行注释
            while (last_char != '\n' && last_char != EOF) {
                last_char = get_character();
            }
            if (last_char != EOF) {
                return get_token();
            }
        } else if (next_char == '*') {  // 多行注释
            last_char = ' ';
            next_char = get_character();
            while (last_char != '*' || next_char != '/') {
                last_char = next_char;
                next_char = get_character();
                if (next_char == EOF) {
                    break;
                }
            }
            if (last_char == '*' && next_char == '/') {
                return get_token();
            }
        } else {  // 不是注释
            unget_character(next_char);
        }
    }

    switch (last_char) {  // 二元运算符
    case '+':
        last_char = get_character();
        return tok_add;
    case '-':
        last_char = get_character();
        return tok_sub;
    case '*':
        last_char = get_character();
        return tok_mul;
    case '/':
        last_char = get_character();
        return tok_div;
    case '&':
        last_char = get_character();
        if (last_char == '&') {
            last_char = get_character();
            return tok_and;
        }
        break;
    case '|':
        last_char = get_character();
        if (last_char == '|') {
            last_char = get_character();
            return tok_or;
        }
        break;
    case '<':
        last_char = get_character();
        if (last_char == '=') {
            last_char = get_character();
            return tok_le;
        }
        return tok_lt;
    case '>':
        last_char = get_character();
        if (last_char == '=') {
            last_char = get_character();
            return tok_ge;
        }
        return tok_gt;
    case '=':
        last_char = get_character();
        if (last_char == '=') {
            last_char = get_character();
            return tok_eq;
        }
        break;
    case '!':
        last_char = get_character();
        if (last_char == '=') {
            last_char = get_character();
            return tok_ne;
        }
    }

    if (last_char == EOF) {
        return tok_eof;
    }

    int cur_char = last_char;
    last_char = get_character();
    return cur_char;
}

int get_next_token() {
    current_token = get_token();
    return current_token;
}