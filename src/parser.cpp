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

Exceptions::ParsingError *Parser::error(const char *message) {
    char buffer[512];
    Position p = getCurrentPosition();
    std::snprintf(buffer, 512, "Error at line %zu, position %zu:\n%s", p.line,
                  p.position, message);
    return new Exceptions::ParsingError(buffer);
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
    // if (match<Token::False>())
    //     return Expression(Literal(LoxFalse()));
    // if (match<Token::True>())
    //     return Expression(Literal(LoxFalse()));
    // if (match<Token::Nil>())
    //     return Expression(Literal(LoxFalse()));
    // if (match<Token::NumberLiteral>())
    //     return Expression(Literal(123.));
    // if (match<Token::StringLiteral>())
    //     return Expression(Literal("asdf"));

    // throw error("expected ')' after expression.");
    Token::Token &t = nextToken();
    std::cout << "Primary " << t << std::endl;
    Expression e = std::visit( //
        Overload{
            [&](auto) -> Expression {
                printf("wtf\n");
                return Expression(Literal("wtf"));
            },
            [&](Token::False) -> Expression {
                printf("False\n");
                return Expression(Literal(LoxFalse()));
            },
            [&](Token::True) -> Expression {
                printf("True\n");
                return Expression(Literal(LoxTrue()));
            },
            [&](Token::Nil) -> Expression {
                printf("Nil\n");
                return Expression(Literal(LoxNil()));
            },
            [](Token::NumberLiteral &number) -> Expression {
                printf("NumberLiteral\n");
                return Expression(Literal(number.value));
            },
            [&](Token::StringLiteral string) -> Expression {
                printf("StringLiteral\n");
                return Expression(Literal(string.value));
            },
            [&](Token::LeftParen) -> Expression {
                std::cout << "LeftParen" << std::endl;
                Expression expr = expression();
                try {
                    consume<Token::RightParen>();
                } catch (std::logic_error) {
                    throw error("expected ')' after expression.");
                }
                return Expression(Grouping(std::move(expr)));
            },
        },
        t);
    return e;
}
