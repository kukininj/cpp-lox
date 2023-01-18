#include "overload.h"
#include <string>
#include <variant>

#ifndef LOXVALUE
#define LOXVALUE

struct LoxNil {};

struct LoxValue : std::variant<double, std::string, bool, LoxNil> {
    using variant::variant;

    template <typename T> T into() const { return std::get<T>(*this); }

    template <typename T> bool is() const {
        return std::holds_alternative<T>(*this);
    }

    const char *getName() const;
};

std::ostream &operator<<(std::ostream &strm, const LoxValue &value);

#endif
