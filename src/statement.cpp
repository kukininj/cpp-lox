#include "statement.h"

std::ostream &operator<<(std::ostream &strm, const Statement &stmt) {
    if (stmt.is<PrintStatement>()) {
        return strm << "PrintStatement: {" << stmt.into<PrintStatement>().expr
                    << "}";
    } else if (stmt.is<ExpressionStatement>()) {
        return strm << "ExpressionStatement: {"
                    << stmt.into<ExpressionStatement>().expr << "}";
    }

    else
        throw Exceptions::NotImplemented("not implemented statement");
    return strm;
};