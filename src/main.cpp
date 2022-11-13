#include <cstdio>
#include <iostream>
#include "token.h"
#include "scanner.h"

void run_prompt() {
    std::cout << "cpp_lox" << std::endl;
    
    bool running = true;
    
    std::string code = "";
    
    while (running) {
        getline(std::cin, code);
        Scanner scanner(code);
        
        for(auto &token : scanner.getTokens()) {
            std::cout << &token << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    if (argc == 1) {
        run_prompt();
    }
}
