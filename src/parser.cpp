#include "parser.h"
#include "loxValue.h"
#include "token.h"
using namespace Expr;

Position Parser::getCurrentPosition() {
    return std::visit(Overload{[&](const Token::BaseToken &token) -> Position {
                          return token.position;
                      }},
                      *iterator.base());
};

Exceptions::ParsingError Parser::error(const char *message) {
    char buffer[512];
    Position p = getCurrentPosition();
    std::snprintf(buffer, 512, "Error at line %zu, position %zu:\n\t%s", p.line,
                  p.position, message);
    return Exceptions::ParsingError(std::string(buffer));
}

template <typename T> void Parser::consume() {
    if (std::holds_alternative<T>(*iterator.base())) {
        iterator++;
    } else {
        throw std::logic_error("");
    }
}

template <typename T> bool Parser::match() {
    // not sure if end() == iterator is necessary
    return tokens.end() == iterator ||
           std::holds_alternative<T>(*iterator.base());
}

Token::Token &Parser::nextToken() { return *(iterator++).base(); };
template <typename T> T &Parser::nextToken() {
    return std::get<T>(*(iterator++).base());
};

Expression Parser::expression() { return equality(); };

Expression Parser::equality() {
    Expression expr = comparison();

    while (match<Token::BangEqual>() || match<Token::EqualEqual>()) {
        Expression left = std::move(expr);
        Token::Token &op = nextToken();
        Expression right = comparison();

        expr = Expression(Binary(std::move(left), op, std::move(right)));
    }

    return expr;
};

Expression Parser::comparison() {
    Expression expr = term();

    while (match<Token::Greater>() || match<Token::GreaterEqual>() ||
           match<Token::Less>() || match<Token::LessEqual>()) {
        Expression left = std::move(expr);
        Token::Token &op = nextToken();
        Expression right = term();

        expr = Expression(Binary(std::move(left), op, std::move(right)));
    }

    return expr;
};

Expression Parser::term() {
    Expression expr = factor();

    while (match<Token::Minus>() || match<Token::Plus>()) {
        Expression left = std::move(expr);
        Token::Token &op = nextToken();
        Expression right = factor();
        expr = Expression(Binary(std::move(left), op, std::move(right)));
    }

    return expr;
};

Expression Parser::factor() {
    Expression expr = unary();

    while (match<Token::Slash>() || match<Token::Star>()) {
        Expression left = std::move(expr);
        Token::Token &op = nextToken();
        Expression right = unary();
        expr = Expression(Binary(std::move(left), op, std::move(right)));
    }

    return expr;
};

Expression Parser::unary() {
    if (match<Token::Minus>() || match<Token::Plus>()) {
        Token::Token &op = nextToken();
        Expression right = primary();
        return Expression(Unary(op, std::move(right)));
    }

    return primary();
};

Expression Parser::primary() {
    if (match<Token::False>()) {
        consume<Token::False>();
        return Expression(Literal(LoxFalse()));
    }
    if (match<Token::True>()) {
        consume<Token::True>();
        return Expression(Literal(LoxTrue()));
    }
    if (match<Token::Nil>()) {
        consume<Token::Nil>();
        return Expression(Literal(LoxNil()));
    }
    if (match<Token::NumberLiteral>()) {
        auto number = nextToken<Token::NumberLiteral>();
        return Expression(Literal(number.value));
    }
    if (match<Token::StringLiteral>()) {
        auto string = nextToken<Token::StringLiteral>();
        return Expression(Literal(string.value));
    }
    if (match<Token::LeftParen>()) {
        consume<Token::LeftParen>();
        Expression expr = expression();
        if (match<Token::RightParen>())
            consume<Token::RightParen>();
        else
            throw error("expected ')' after expression.");

        return Expression(Grouping(std::move(expr)));
    }
    throw error("expected Primary.");
}