#include "expression.h"
#include "interpreter.h"
#include "parser.h"
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

    Interpreter interpeter;

    while (running) {
        printf(">> ");
        getline(std::cin, code);
        Scanner scanner(code);

        try {
            auto tokens = scanner.getTokens();
            // for (auto &token : tokens) {
            //     std::cout << token << std::endl;
            // }

            Parser parser(tokens);
            Expression expr = parser.expression();

            std::cout << expr << std::endl;

            LoxValue result = interpeter.interpret(expr);
            std::cout << result << std::endl;

        } catch (Exceptions::SyntaxError err) {
            std::cout << err.what() << std::endl;
        } catch (Exceptions::ParsingError err) {
            std::cout << err.what() << std::endl;
        } catch (Exceptions::RuntimeException err) {
            std::cout << err.what() << std::endl;
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

        try {
            auto tokens = scanner.getTokens();
            for (auto &token : tokens) {
                std::cout << token << std::endl;
            }
            Parser parser(tokens);
            Expression expr = parser.expression();

            std::cout << expr << std::endl;

        } catch (std::exception e) {
            std::cout << e.what() << std::endl;
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
