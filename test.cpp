#include "src/lexer/Lexer.h"
#include <iostream>

int main(int argc, char *argv[]) {
    while (true) {
        std::cout << "$>";
        std::cout << get_token() << std::endl;
    }
    return 0;
}
