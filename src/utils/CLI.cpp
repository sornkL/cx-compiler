#include "CLI.h"

bool is_input_file;
bool is_optimize;
std::string input_filename;
std::string output_filename;

std::ifstream input_file;

/**
 * cxc -i [-O]
 * cxc -o <output_filename.ll> <input_filename.cx> [-O]
 */
void parse_command(int argc, char *argv[]) {
    for (int i=1; i<argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'i':
                // -i 表示在命令行下使用交互模式
                is_input_file = false;
                break;
            case 'o':
                // -o 表示输入是待编译的文件，并给出输出IR的文件名
                is_input_file = true;
                output_filename = argv[++i];
                input_filename = argv[++i];
                break;
            case 'O':
                // -O 表示优化IR
                is_optimize = true;
                break;
            }
        }
    }

    if (is_input_file) {
        input_file.open(input_filename);
    }
}

void close_file() {
    if (is_input_file) {
        if (input_file.is_open()) {
            input_file.close();
        }
    }
}