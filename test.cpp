#include "src/lexer/Lexer.h"
#include "src/parser/Parser.h"
#include "src/cx/CX.h"
#include <iostream>

static void initialize_module() {
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    modules = std::make_unique<llvm::Module>("cx compiler", *context);
}

static void handle_top_level_expression() {
    if (auto function_ast = parse_top_level_expression()) {
        if (auto *function_ir = function_ast->codegen()) {
            std::cout << "Read top-level expression:" << std::endl;
            function_ir->print(llvm::errs());
            std::cout << std::endl;

            // function_ir->eraseFromParent();
        }
    } else {
        get_next_token();
    }
}

static void main_loop() {
    while (true) {
        std::cout << ">>> ";
        switch (current_token) {
            case tok_eof:
                return;
            case ';':
                get_next_token();
                break;
            default:
                handle_top_level_expression();
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    std::cout << ">>> ";
    get_next_token();
    initialize_module();

    main_loop();

    modules->print(llvm::errs(), nullptr);

    return 0;
}
