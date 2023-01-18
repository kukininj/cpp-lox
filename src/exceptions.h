#include <exception>
#include <iostream>

#ifndef EXCEPT
#define EXCEPT

#define EXCEPTION(NAME)                                                        \
    class NAME : public std::logic_error {                                     \
        using logic_error::logic_error;                                        \
    };

namespace Exceptions {
EXCEPTION(NotImplemented)
EXCEPTION(SyntaxError)
EXCEPTION(ParsingError)
EXCEPTION(RuntimeException)

} // namespace Exceptions

#endif