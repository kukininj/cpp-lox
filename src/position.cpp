#include "position.h"

std::ostream &operator<<(std::ostream &strm, const Position &a) {
    return strm << "line: " << a.line << ", position: " << a.position;
}