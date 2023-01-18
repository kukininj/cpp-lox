#ifndef INTERPRETER
#define INTERPRETER

#include "expression.h"
#include "loxValue.h"

using namespace Expr;

class Interpreter {

    LoxValue visitExpression(const Expression &expression);

    LoxValue visitUnary(const Unary &expression);
    LoxValue visitBinary(const Binary &expression);
    LoxValue visitGrouping(const Grouping &expression);
    LoxValue visitLiteral(const Literal &expression);

  public:
    LoxValue interpret(const Expression &expression);
};

#endif
