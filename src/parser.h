

#ifndef PARSER
#define PARSER

#include "exceptions.h"
#include "expression.h"
#include "overload.h"
#include "position.h"
#include "statement.h"
#include "token.h"
#include <stdexcept>
#include <vector>

using namespace Expr;

class Parser {
    std::vector<Token::Token> tokens;
    std::vector<Token::Token>::iterator iterator;

    Position getCurrentPosition();
    Exceptions::ParsingError error(const char *message);

    bool isAtEnd();

    template <typename T> void consume();
    template <typename T> bool match();

    Token::Token &nextToken();
    template <typename T> T &nextToken();

    Expression expression();
    Expression equality();
    Expression comparison();
    Expression term();
    Expression factor();
    Expression unary();
    Expression primary();

    Statement statement();
    Statement printStatement();
    Statement expressionStatement();

  public:
    Parser(std::vector<Token::Token> tokens) : tokens{tokens} {
        iterator = this->tokens.begin();
    };
    std::vector<Statement> parse();
};

#endif
