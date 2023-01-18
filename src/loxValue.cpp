#include "loxValue.h"
#include <iostream>

std::ostream &operator<<(std::ostream &strm, const LoxValue &value) {
    return std::visit(
        Overload{[&strm](const double &number) -> std::ostream & {
                     return strm << number;
                 },
                 [&strm](const std::string &string) -> std::ostream & {
                     return strm << string;
                 },
                 [&strm](const bool &value) -> std::ostream & {
                     return strm << (value ? "True" : "False");
                 },
                 [&strm](const LoxNil &) -> std::ostream & {
                     return strm << "Nil";
                 }},
        value);
};
const char *LoxValue::getName() const {
    // clang-format off
    return std::visit( 
    Overload{ [](const double &number) -> auto { return "Number"; }
            , [](const std::string &string) -> auto{ return "String"; } 
            , [](const bool &value) -> auto{ return "Boolean"; }
            , [](const LoxNil &) -> auto{ return "Null"; }
    }, *this); // clang-format on
}