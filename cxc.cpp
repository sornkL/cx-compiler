#include "src/lexer/Lexer.h"
#include "src/parser/Parser.h"
#include "src/cx/CX.h"
#include "include/CXJIT.h"
#include "utils/CLI.h"
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils.h>
#include <llvm/Support/FileSystem.h>
#include <iostream>
#include <fstream>

static llvm::ExitOnError exit_on_error;

static void initialize_module() {
    context = std::make_unique<llvm::LLVMContext>();
    modules = std::make_unique<llvm::Module>("cx compiler", *context);
    modules->setDataLayout(jit->get_data_layout());

    builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

static void handle_top_level_expression() {
    if (auto function_ast = parse_top_level_expression()) {
        if (auto *function_ir = function_ast->codegen()) {
            if (!is_input_file) {
                std::cout << "Read top-level expression:" << std::endl;
                function_ir->print(llvm::errs());
                std::cout << std::endl;
            } else {
                std::error_code ec;
                llvm::raw_fd_ostream output_file(output_filename, ec, llvm::sys::fs::OF_Append);

                function_ir->print(output_file, nullptr);
                output_file << "\n";
                output_file.close();
            }

            auto rt = jit->get_main_jit_dylib().createResourceTracker();
            auto tsm = llvm::orc::ThreadSafeModule(std::move(modules), std::move(context));
            exit_on_error(jit->add_module(std::move(tsm), rt));
            // initialize_module();

            auto expr_symbol = jit->lookup("main");
            void (*fp)() = (void (*)())expr_symbol->getAddress();
            // std::cout << "Evaluated to " << fp() << std::endl;
            fp();

            exit_on_error(rt->remove());

            // function_ir->removeFromParent();
        }
    } else {
        get_next_token();
    }
}

static void handle_function_definition() {
    if (auto function_ast = parse_function_definition()) {
        if (auto *function_ir = function_ast->codegen()) {
            if (!is_input_file) {
                std::cout << "Read function definition:" << std::endl;
                function_ir->print(llvm::errs());
                std::cout << std::endl;
            } else {
                std::error_code ec;
                llvm::raw_fd_ostream output_file(output_filename, ec, llvm::sys::fs::OF_Append);

                function_ir->print(output_file, nullptr);
                output_file << "\n";
                output_file.close();
            }
            
            exit_on_error(jit->add_module(llvm::orc::ThreadSafeModule(std::move(modules), std::move(context))));

            initialize_module();
            
            // function_ir->removeFromParent();
        }
    } else {
        get_next_token();
    }
}

static void main_loop() {
    while (true) {
        if (!is_input_file) {
            std::cout << ">>> ";
        }
        switch (current_token) {
            case tok_eof:
                return;
            case tok_func:
                handle_function_definition();
                break;
            case ';':
                get_next_token();
                break;
            case '{':
                handle_top_level_expression();
                break;
            case '}':
                get_next_token();
                break;
            default:
                handle_top_level_expression();
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    parse_command(argc, argv);

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    if (!is_input_file) {
        std::cout << ">>> ";
    }
    get_next_token();

    jit = exit_on_error(llvm::orc::CXJIT::create());
    initialize_module();

    if (is_input_file) {
        std::error_code ec;
        llvm::raw_fd_ostream output_file(output_filename, ec, llvm::sys::fs::OF_Text);

        modules->print(output_file, nullptr);
        output_file << "\n";
        output_file.close();
    }

    main_loop();
    
    close_file();

    return 0;
}
