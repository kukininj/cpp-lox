#include "expression.h"
#include "loxValue.h"
#include "statement.h"

#ifndef INTERPRETER
#define INTERPRETER

using namespace Expr;

class Interpreter {

    LoxValue visitExpression(const Expression &expression);

    LoxValue visitUnary(const Unary &expression);
    LoxValue visitBinary(const Binary &expression);
    LoxValue visitGrouping(const Grouping &expression);
    LoxValue visitLiteral(const Literal &expression);

    LoxValue visitStateent(const Statement &statement);
    LoxValue visitPrintStateent(const PrintStatement &statement);
    LoxValue visitExpressionStateent(const ExpressionStatement &statement);

  public:
    LoxValue interpret(const Statement &expression);
};

#endif
