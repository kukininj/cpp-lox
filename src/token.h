#ifndef TOKEN
#define TOKEN

#include "position.h"
#include "overload.h"
#include <iostream>
#include <optional>
#include <variant>

namespace Token {
typedef std::variant<std::string, double> Literal;

struct BaseToken {
    std::string lexeme;
    Position position;
    explicit BaseToken(std::string lexeme, Position position)
        : lexeme{lexeme}, position{position} {};
};

struct Minus : BaseToken {
    using BaseToken::BaseToken;
};
struct Plus : BaseToken {
    using BaseToken::BaseToken;
};
struct Slash : BaseToken {
    using BaseToken::BaseToken;
};
struct Star : BaseToken {
    using BaseToken::BaseToken;
};
struct Bang : BaseToken {
    using BaseToken::BaseToken;
};
struct BangEqual : BaseToken {
    using BaseToken::BaseToken;
};
struct Equal : BaseToken {
    using BaseToken::BaseToken;
};
struct EqualEqual : BaseToken {
    using BaseToken::BaseToken;
};
struct Greater : BaseToken {
    using BaseToken::BaseToken;
};
struct GreaterEqual : BaseToken {
    using BaseToken::BaseToken;
};
struct Less : BaseToken {
    using BaseToken::BaseToken;
};
struct LessEqual : BaseToken {
    using BaseToken::BaseToken;
};

struct Semicolon : BaseToken {
    using BaseToken::BaseToken;
};
struct LeftParen : BaseToken {
    using BaseToken::BaseToken;
};
struct RightParen : BaseToken {
    using BaseToken::BaseToken;
};
struct LeftBrace : BaseToken {
    using BaseToken::BaseToken;
};
struct RightBrace : BaseToken {
    using BaseToken::BaseToken;
};
struct Comma : BaseToken {
    using BaseToken::BaseToken;
};
struct Dot : BaseToken {
    using BaseToken::BaseToken;
};
struct Eof : BaseToken {
    using BaseToken::BaseToken;
};

struct And : BaseToken {
    using BaseToken::BaseToken;
};
struct Class : BaseToken {
    using BaseToken::BaseToken;
};
struct Else : BaseToken {
    using BaseToken::BaseToken;
};
struct False : BaseToken {
    using BaseToken::BaseToken;
};
struct Fun : BaseToken {
    using BaseToken::BaseToken;
};
struct For : BaseToken {
    using BaseToken::BaseToken;
};
struct If : BaseToken {
    using BaseToken::BaseToken;
};
struct Nil : BaseToken {
    using BaseToken::BaseToken;
};
struct Or : BaseToken {
    using BaseToken::BaseToken;
};
struct Print : BaseToken {
    using BaseToken::BaseToken;
};
struct Return : BaseToken {
    using BaseToken::BaseToken;
};
struct Super : BaseToken {
    using BaseToken::BaseToken;
};
struct This : BaseToken {
    using BaseToken::BaseToken;
};
struct True : BaseToken {
    using BaseToken::BaseToken;
};
struct Var : BaseToken {
    using BaseToken::BaseToken;
};
struct While : BaseToken {
    using BaseToken::BaseToken;
};

struct Identifier : BaseToken {
    using BaseToken::BaseToken;
    std::string value;
    Identifier(std::string value, std::string lexeme, Position position)
        : BaseToken{lexeme, position}, value(value){};
};
struct StringLiteral : BaseToken {
    using BaseToken::BaseToken;
    std::string value;
    StringLiteral(std::string value, std::string lexeme, Position position)
        : BaseToken{lexeme, position}, value(value){};
};
struct NumberLiteral : BaseToken {
    using BaseToken::BaseToken;
    double value;
    NumberLiteral(double value, std::string lexeme, Position position)
        : BaseToken{lexeme, position}, value(value){};
};

struct Token : std::variant<Identifier, StringLiteral, NumberLiteral,

                            LeftParen, RightParen, LeftBrace, RightBrace, Comma,
                            Semicolon, Dot,

                            Minus, Plus, Slash, Star,

                            Bang, BangEqual, Equal, EqualEqual, Greater,
                            GreaterEqual, Less, LessEqual,

                            And, Class, Else, False, Fun, For, If, Nil, Or,
                            Print, Return, Super, This, True, Var, While,

                            Eof> {
    using variant::variant;
    Position getPosition() const;
    template <typename T> bool is() const {
        return std::holds_alternative<T>(*this);
    }
};

const char *getName(const Token &token);
std::ostream &operator<<(std::ostream &strm, const Token &a);
} // namespace Token
#endif