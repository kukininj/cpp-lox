#ifndef EXPRESSION
#define EXPRESSION

#include "loxValue.h"
#include "overload.h"
#include "token.h"
#include <iostream>
#include <memory>
#include <variant>
namespace Expr {

struct ExpressionBase;
struct Expression {
    std::unique_ptr<ExpressionBase> base;
    Expression(ExpressionBase &&base) : base(nullptr) {
        this->base = std::make_unique<ExpressionBase>(std::move(base));
    }

};
std::ostream &operator<<(std::ostream &strm, const Expression& expr);

struct Unary {
    Expression expr;
    Token::Token token;
    LoxValue interpret();
    Unary(Token::Token token, Expression &&expr)
        : expr{std::move(expr)}, token{token} {};
};

struct Binary {
    Expression left;
    Token::Token token;
    Expression right;
    Binary(Expression &&left, Token::Token token, Expression &&right)
        : left{std::move(left)}, token{token}, right{std::move(right)} {};
    LoxValue interpret();
};

struct Grouping {
    Expression expr;
    LoxValue interpret();
    Grouping(Expression &&expr) : expr(std::move(expr)){};
};

struct Literal {
    LoxValue value;
    LoxValue interpret();
    Literal(LoxValue value) : value(value){};
};

struct ExpressionBase : std::variant<Unary, Binary, Grouping, Literal> {
    using variant::variant;

    LoxValue interpret();
};

} // namespace Expr

#endif