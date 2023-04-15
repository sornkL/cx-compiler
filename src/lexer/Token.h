enum Token {
    tok_eof = -1,

    // 关键字
    tok_int = -2,
    tok_bool = -3,
    tok_if = -4,
    tok_else = -5,
    tok_while = -6,
    tok_write = -7,
    tok_read = -8,
    tok_true = -9,
    tok_false = -10,

    // 标识符与数字
    tok_identifier = -20,
    tok_integer_number = -21,
    tok_float_number = -22
};
