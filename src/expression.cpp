#include "expression.h"

using namespace Expr;

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

const char *Expr::Expression::getName() const {
    return std::visit( //
        Overload{
            [](const Unary &) -> const char * { return "Unary"; },
            [](const Binary &) -> const char * { return "Binary"; },
            [](const Grouping &) -> const char * { return "Grouping"; },
            [](const Literal &) -> const char * { return "Literal"; },
        },
        *base.get());
};
