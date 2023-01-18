#include "expression.h"
#include <variant>

#ifndef STATEMENT
#define STATEMENT

using Expr::Expression;

struct PrintStatement {
    Expression expr;
    PrintStatement(Expression &&expr) : expr{std::move(expr)} {};
};
struct ExpressionStatement {
    Expression expr;
    ExpressionStatement(Expression &&expr) : expr{std::move(expr)} {};
};

class Statement : std::variant<PrintStatement, ExpressionStatement> {
    using variant::variant;

  public:
    template <typename T> const T &into() const { return std::get<T>(*this); }

    template <typename T> bool is() const {
        return std::holds_alternative<T>(*this);
    }
};
std::ostream &operator<<(std::ostream &strm, const Statement &stmt);

#endif