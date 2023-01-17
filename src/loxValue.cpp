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
                 [&strm](const LoxTrue&) -> std::ostream & {
                     return strm << "True";
                 },
                 [&strm](const LoxFalse&) -> std::ostream & {
                     return strm << "False";
                 },
                 [&strm](const LoxNil&) -> std::ostream & {
                     return strm << "Nil";
                 }},
        value);
};