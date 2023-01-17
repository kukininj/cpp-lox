

#ifndef PARSER
#define PARSER

#include "exceptions.h"
#include "expression.h"
#include "overload.h"
#include "position.h"
#include "token.h"
#include <stdexcept>
#include <vector>

using namespace Expr;

class Parser {
    std::vector<Token::Token> tokens;
    std::vector<Token::Token>::iterator iterator;

    Position getCurrentPosition();
    Exceptions::ParsingError error(const char *message);

    template <typename T> void consume();
    template <typename T> bool match();

    Token::Token &nextToken();
    template <typename T> T &nextToken();

    Expression equality();
    Expression comparison();
    Expression term();
    Expression factor();
    Expression unary();
    Expression primary();

  public:
    Parser(std::vector<Token::Token> tokens) : tokens{tokens} {
        iterator = this->tokens.begin();
    };
    Expression expression();
};

#endif
