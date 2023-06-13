#ifndef __CLI_H__
#define __CLI_H__
#include <string>
#include <iostream>
#include <fstream>

extern bool is_input_file;
extern bool is_optimize;
extern std::string input_filename;
extern std::string output_filename;

extern std::ifstream input_file;

void parse_command(int argc, char *argv[]);

void close_file();

#endif