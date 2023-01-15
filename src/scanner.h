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

    void advanceLinePosition();

    Position getPosition() const;
    std::string getLexeme() const;

    char consumeCharacter();
    bool match(char c);
    void skipComment();
    void skipWhitespaceCharacters();
    void scanStringLiteral();
    void scanNumberLiteral();
    void scanIdentifier();

    template <typename T> void addToken() {
        this->tokens.push_back(T{getLexeme(), getPosition()});
    }
    void scanToken();
    void addToken(Token::Literal literal);
    void addIdentifier(std::string name);

    template <typename T> static Token::Token createKeyword(const Scanner &s) {
        return *(new T(s.getLexeme(), s.getPosition()));
    }

  public:
    Scanner(std::string source);

    std::vector<Token::Token> getTokens();
};
#endif
