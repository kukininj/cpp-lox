#include <exception>
#include <iostream>

#ifndef EXCEPT
#define EXCEPT


#define EXCEPTION(NAME) class NAME : public std::logic_error { public: NAME(const std::string& message) : std::logic_error(message) { }; };

namespace Exceptions {
EXCEPTION(NotImplemented)
EXCEPTION(SyntaxError)
EXCEPTION(ParsingError)

}

#endif