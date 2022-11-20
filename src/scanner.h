#include "position.h"
#include "token.h"

#include <iostream>
#include <vector>

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
    char currentCharacter();
    char nextCharacter();
    
    Position getPosition();

    char consumeCharacter();
    bool match(char c);
    void skipComment();
    void skipWhitespaceCharacters();
    void scanStringLiteral();
    void scanNumberLiteral();
    void scanIdentifier();

    void scanToken();
    void addToken(TokenType type);
    void addToken(Token::Literal literal);
    void addIdentifier(std::string name);

  public:
    Scanner(std::string source);

    std::vector<Token::Token> getTokens();
};
#endif
