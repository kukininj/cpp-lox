#include "token.h"

#include <vector>
#include <iostream>


#ifndef SCANNER
#define SCANNER

class Scanner {
    size_t lineNumber;
    size_t linePosition;
    size_t sourcePosition;
    
    std::string source;
    
    bool isAtEnd();
    Token::Token scanToken();

    public:
    Scanner(std::string source);

    std::vector<Token::Token> getTokens();
};
#endif
