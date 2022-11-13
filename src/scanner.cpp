#include "scanner.h"
#include "token.h"
#include <vector>

bool Scanner::isAtEnd() {
    return false;
}

Scanner::Scanner(std::string source) { this->source = source; }


std::vector<Token::Token> Scanner::getTokens() {
    std::vector<Token::Token> tokens;

    while (!isAtEnd()) {
        
    }
    
    return tokens;
}
