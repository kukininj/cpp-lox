#include <exception>
#include <iostream>

#define EXCEPTION(NAME) class NAME : public std::logic_error { public: NAME(const char* message) : std::logic_error(message) { }; };

namespace Exceptions {
EXCEPTION(NotImplemented)
EXCEPTION(SyntaxError)
}

#undef EXCEPTION