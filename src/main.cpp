#include "scanner.h"
#include "token.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

void run_prompt() {
    std::cout << "cpp_lox" << std::endl;

    bool running = true;

    std::string code = "";

    while (running) {
        getline(std::cin, code);
        Scanner scanner(code);

        for (auto &token : scanner.getTokens()) {
            std::cout << &token << std::endl;
        }
    }
}

void run_file(std::string filename) {
    std::string source;
    std::ifstream source_file(filename);

    if (source_file) {
        std::ostringstream stream;
        stream << source_file.rdbuf();
        source = stream.str();
        Scanner scanner(source);

        auto tokens = scanner.getTokens();
        for (auto &token : tokens) {
            std::cout << token << std::endl;
        }
    } else {
        printf("Could not read file %s", filename.c_str());
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        run_prompt();
    } else if (argc == 2) {
        run_file(argv[1]);
    }
}
