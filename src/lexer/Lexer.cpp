#include "Lexer.h"
#include <iostream>
#include <fstream>

int current_token;
bool skip_get_next_token;

std::string identifier;
int integer_number;
float float_number;
bool boolean;

int lineno;

int get_character() {
    int cur_char;
    if (is_input_file) {
        cur_char = input_file.get();
    } else {
        cur_char = getchar();
    }
    if (cur_char == '\n') {
        lineno++;
    }
    return cur_char;
}

void unget_character(int c) {
    if (is_input_file) {
        input_file.unget();
    } else {
        ungetc(c, stdin);
    }
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
            boolean = true;
            return tok_true;
        }
        if (identifier == "false") {
            boolean = false;
            return tok_false;
        }
        if (identifier == "float") {
            return tok_float;
        }
        if (identifier == "func") {
            return tok_func;
        }
        if (identifier == "return") {
            return tok_return;
        }
        if (identifier == "for") {
            return tok_for;
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
        last_char = get_character();
        if (last_char == '/') {  // 单行注释
            do {
                last_char = get_character();
            } while (last_char != EOF && last_char != '\n');

            if (last_char != EOF) {
                return get_token();
            }
        } else if (last_char == '*') {  // 多行注释
            do {
                last_char = get_character();
                if (last_char == '*') {
                    last_char = get_character();
                    if (last_char == '/') {
                        last_char = get_character();
                        return get_token();
                    }
                }
            } while (last_char != EOF);
        } else {
            return tok_div;
        }
    }

    switch (last_char) {  // 二元运算符
    case '+':
        last_char = get_character();
        return tok_add;
    case '-':
        last_char = get_character();
        if (last_char == '>') {
            last_char = get_character();
            return tok_arrow;
        }
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
        return tok_assign;
    case '!':
        last_char = get_character();
        if (last_char == '=') {
            last_char = get_character();
            return tok_ne;
        }
        return tok_not;
    case '%':
        last_char = get_character();
        return tok_mod;
    case '^':
        last_char = get_character();
        return tok_xor;
    }

    if (last_char == EOF) {
        return tok_eof;
    }

    int cur_char = last_char;
    last_char = get_character();
    return cur_char;
}

int get_next_token() {
    if (!skip_get_next_token) {
        current_token = get_token();
    } else {
        skip_get_next_token = false;
    }
    return current_token;
}
