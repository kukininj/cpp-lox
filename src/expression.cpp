#include "expression.h"

using namespace Expr;

LoxValue Expr::Unary::interpret() { return 1.; }

LoxValue Expr::Binary::interpret() { return 2.; }

LoxValue Expr::Grouping::interpret() { return 2.; }

LoxValue Expr::Literal::interpret() { return value; };

LoxValue Expr::ExpressionBase::interpret() {
    return std::visit(
        Overload{[](auto &b) -> LoxValue { return b.interpret(); }}, *this);
}

std::ostream &Expr::operator<<(std::ostream &strm, const Expression &expr) {
    return std::visit( //
        Overload{
            [&strm](const Unary &unary) -> std::ostream & {
                return strm << "Unary: {"
                            << "expr: " << unary.expr << ","
                            << "op: " << unary.token << "}";
            },
            [&](const Binary &binary) -> std::ostream & {
                return strm << "Binary: {"
                            << "left: " << binary.left << ","
                            << "op: " << binary.token << ","
                            << "right: " << binary.right << "}";
            },
            [&](const Grouping &grouping) -> std::ostream & {
                return strm << "Grouping: {"
                            << "expr: " << grouping.expr << "}";
            },
            [&](const Literal &literal) -> std::ostream & {
                return strm << "Literal: {"
                            << "value: " << literal.value << "}";
            },
        },
        *expr.base.get());
};