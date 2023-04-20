#include "src/lexer/Lexer.h"
#include "src/parser/Parser.h"
#include <iostream>

int main(int argc, char *argv[]) {
    while (true) {
        std::cout << "$>";
        get_next_token();
        std::cout << "current token: " << current_token << std::endl;
    }
    return 0;
}
