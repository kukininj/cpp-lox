#include "interpreter.h"
#include <cstdio>
#include <iostream>
#include <optional>
#include <sstream>

using Exceptions::RuntimeException;

RuntimeException runtime_error(const char *message, const Token::Token &token) {
    char buffer[512] = {'\0'};

    Position p = token.getPosition();
    std::snprintf(buffer, 512, "Error at line %zu, position %zu:\n\t%s", p.line,
                  p.position, message);
    return RuntimeException(std::string(buffer));
}

LoxValue Interpreter::interpret(const Statement &stmt) {
    return visitStateent(stmt);
};

LoxValue Interpreter::visitUnary(const Unary &expression) {
    LoxValue value = visitExpression(expression.expr);
    if (expression.token.is<Token::Minus>()) {
        try {
            return -value.into<double>();
        } catch (std::bad_variant_access) {
            throw runtime_error("expected number", expression.token);
        }
    } else if (expression.token.is<Token::Bang>()) {
        try {
            return !value.into<bool>();
        } catch (std::bad_variant_access) {
            throw runtime_error("expected boolean", expression.token);
        }
    }
    throw runtime_error("unnkown Unary token", expression.token);
};

Exceptions::RuntimeException
binary_op_error(const char *op, const LoxValue &left, const LoxValue &right) {
    std::stringstream message;
    char buff[4] = {'\0'};
    snprintf(buff, 4, "'%s'", op);
    message << buff                                       //
            << " operator cant handle given operands: \n" //
            << "\tleft: " << left.getName() << std::endl  //
            << "\tright: " << right.getName() << std::endl;

    throw Exceptions::RuntimeException(message.str());
}

LoxValue operator+(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() + right.into<double>();
    }
    if (left.is<std::string>() && right.is<std::string>()) {
        return left.into<std::string>() + right.into<std::string>();
    }

    throw binary_op_error("+", left, right);
}

LoxValue operator-(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() - right.into<double>();
    }
    throw binary_op_error("+", left, right);
}

LoxValue operator*(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() * right.into<double>();
    }
    throw binary_op_error("*", left, right);
}

LoxValue operator/(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() - right.into<double>();
    }
    throw binary_op_error("/", left, right);
}

LoxValue operator==(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() == right.into<double>();
    } else if (left.is<bool>() && right.is<bool>()) {
        return left.into<bool>() == right.into<bool>();
    } else if (left.is<std::string>() && right.is<std::string>()) {
        return left.into<std::string>() == right.into<std::string>();
    } else if (left.is<LoxNil>() && right.is<LoxNil>()) {
        return true;
    }
    return false;
}

LoxValue operator<=(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() <= right.into<double>();
    }
    throw binary_op_error("<=", left, right);
}

LoxValue operator>=(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() >= right.into<double>();
    }
    throw binary_op_error(">=", left, right);
}

LoxValue operator<(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() < right.into<double>();
    }
    throw binary_op_error("<", left, right);
}

LoxValue operator>(LoxValue &left, LoxValue &right) {
    if (left.is<double>() && right.is<double>()) {
        return left.into<double>() > right.into<double>();
    }
    throw binary_op_error(">", left, right);
}

LoxValue Interpreter::visitBinary(const Binary &expression) {
    LoxValue left = visitExpression(expression.left);
    LoxValue right = visitExpression(expression.right);
    if (expression.token.is<Token::Plus>()) {
        return left + right;
    } else if (expression.token.is<Token::Minus>()) {
        return left - right;
    } else if (expression.token.is<Token::Star>()) {
        return left * right;
    } else if (expression.token.is<Token::Slash>()) {
        return left / right;
    } else if (expression.token.is<Token::EqualEqual>()) {
        return left == right;
    } else if (expression.token.is<Token::GreaterEqual>()) {
        return left >= right;
    } else if (expression.token.is<Token::LessEqual>()) {
        return left <= right;
    } else if (expression.token.is<Token::Greater>()) {
        return left > right;
    } else if (expression.token.is<Token::Less>()) {
        return left < right;
    } else
        throw runtime_error("unnkown Binary token", expression.token);
};

LoxValue Interpreter::visitLiteral(const Literal &expression) {
    return expression.value;
};

LoxValue Interpreter::visitGrouping(const Grouping &expression) {
    return visitExpression(expression.expr);
};
LoxValue Interpreter::visitExpression(const Expression &expression) {
    if (expression.is<Unary>()) {
        return visitUnary(expression.into<Unary>());
    } else if (expression.is<Binary>()) {
        return visitBinary(expression.into<Binary>());
    } else if (expression.is<Grouping>()) {
        return visitGrouping(expression.into<Grouping>());
    } else if (expression.is<Literal>()) {
        return visitLiteral(expression.into<Literal>());
    }

    throw RuntimeException("unhandled Expression type");
};
LoxValue Interpreter::visitStateent(const Statement &statement) {
    if (statement.is<PrintStatement>()) {
        return visitPrintStateent(statement.into<PrintStatement>());
    } else if (statement.is<ExpressionStatement>()) {
        return visitExpressionStateent(statement.into<ExpressionStatement>());
    }

    else
        throw Exceptions::NotImplemented("not implemented statement");
};
LoxValue Interpreter::visitPrintStateent(const PrintStatement &statement) {
    LoxValue value = visitExpression(statement.expr);
    std::cout << value << std::endl;
    return LoxNil();
};
LoxValue
Interpreter::visitExpressionStateent(const ExpressionStatement &statement) {
    return visitExpression(statement.expr);
};