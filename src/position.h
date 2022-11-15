
#ifndef POSITION
#define POSITION

#include <cstddef>
#include <ostream>
struct Position {
    size_t line;
    size_t position;
};

std::ostream &operator<<(std::ostream &strm, const Position &a);
#endif
