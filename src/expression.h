#include "exceptions.h"
#include "loxValue.h"
#include "overload.h"
#include "token.h"
#include <iostream>
#include <memory>
#include <variant>

#ifndef EXPRESSION
#define EXPRESSION

namespace Expr {

struct ExpressionBase;
struct Expression {
    std::unique_ptr<ExpressionBase> base;
    Expression(ExpressionBase &&base) : base(nullptr) {
        this->base = std::make_unique<ExpressionBase>(std::move(base));
    }

    template <typename T> T *get_if() const {
        return std::get_if<T>(base.get());
    }
    template <typename T> const T& into() const { return std::get<T>(*base.get()); }

    template <typename T> bool is() const {
        return std::holds_alternative<T>(*base.get());
    }

    const char *getName() const;
};
std::ostream &operator<<(std::ostream &strm, const Expression &expr);

struct Unary {
    Expression expr;
    Token::Token token;
    Unary(Token::Token token, Expression &&expr)
        : expr{std::move(expr)}, token{token} {};
};

struct Binary {
    Expression left;
    Token::Token token;
    Expression right;
    Binary(Expression &&left, Token::Token token, Expression &&right)
        : left{std::move(left)}, token{token}, right{std::move(right)} {};
};

struct Grouping {
    Expression expr;
    Grouping(Expression &&expr) : expr(std::move(expr)){};
};

struct Literal {
    LoxValue value;
    Literal(LoxValue value) : value(value){};
};

struct ExpressionBase : std::variant<Unary, Binary, Grouping, Literal> {
    using variant::variant;
};

} // namespace Expr

#endif