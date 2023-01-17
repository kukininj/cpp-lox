#ifndef LOXVALUE
#define LOXVALUE

#include "overload.h"
#include <string>
#include <variant>

struct LoxTrue {};
struct LoxFalse {};
struct LoxNil {};

struct LoxValue : std::variant<double, std::string, LoxTrue, LoxFalse, LoxNil> {
    using variant::variant;
};

std::ostream &operator<<(std::ostream &strm, const LoxValue &value);

#endif
