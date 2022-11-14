#include "token.h"

#include <vector>
#include <iostream>


#ifndef SCANNER
#define SCANNER

class Scanner {
    size_t lineNumber;
    size_t linePosition;
    size_t lexemeStart;
    size_t sourcePosition;
    
    std::string source;
    
    std::vector<Token::Token> tokens;
    
    bool isAtEnd();
    char consumeCharacter();
    bool match(char c);
    void skipComment();
    void skipWhitespaceCharacters();

    void scanToken();
    void addToken(TokenType type);
    public:
    Scanner(std::string source);

    std::vector<Token::Token> getTokens();
};
#endif
