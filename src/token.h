#ifndef TOKEN
#define TOKEN

#include "position.h"
#include <iostream>
#include <optional>
#include <variant>

namespace Token {
typedef std::variant<std::string, double> Literal;

struct BaseToken {
    // TokenType type;
    std::string lexeme;
    // std::optional<Literal> literal;
    Position position;
    explicit BaseToken(std::string lexeme, Position position)
        : lexeme{lexeme}, position{position} {};
};

struct Operator : BaseToken {
    using BaseToken::BaseToken;
};
struct Minus : Operator {
    using Operator::Operator;
};
struct Plus : Operator {
    using Operator::Operator;
};
struct Slash : Operator {
    using Operator::Operator;
};
struct Star : Operator {
    using Operator::Operator;
};
struct Bang : Operator {
    using Operator::Operator;
};
struct BangEqual : Operator {
    using Operator::Operator;
};
struct Equal : Operator {
    using Operator::Operator;
};
struct EqualEqual : Operator {
    using Operator::Operator;
};
struct Greater : Operator {
    using Operator::Operator;
};
struct GreaterEqual : Operator {
    using Operator::Operator;
};
struct Less : Operator {
    using Operator::Operator;
};
struct LessEqual : Operator {
    using Operator::Operator;
};

struct Special : BaseToken {
    using BaseToken::BaseToken;
};
struct Semicolon : Special {
    using Special::Special;
};
struct LeftParen : Special {
    using Special::Special;
};
struct RightParen : Special {
    using Special::Special;
};
struct LeftBrace : Special {
    using Special::Special;
};
struct RightBrace : Special {
    using Special::Special;
};
struct Comma : Special {
    using Special::Special;
};
struct Dot : Special {
    using Special::Special;
};
struct Eof : Special {
    using Special::Special;
};

struct Keyword : BaseToken {
    using BaseToken::BaseToken;
};
struct And : Keyword {
    using Keyword::Keyword;
};
struct Class : Keyword {
    using Keyword::Keyword;
};
struct Else : Keyword {
    using Keyword::Keyword;
};
struct False : Keyword {
    using Keyword::Keyword;
};
struct Fun : Keyword {
    using Keyword::Keyword;
};
struct For : Keyword {
    using Keyword::Keyword;
};
struct If : Keyword {
    using Keyword::Keyword;
};
struct Nil : Keyword {
    using Keyword::Keyword;
};
struct Or : Keyword {
    using Keyword::Keyword;
};
struct Print : Keyword {
    using Keyword::Keyword;
};
struct Return : Keyword {
    using Keyword::Keyword;
};
struct Super : Keyword {
    using Keyword::Keyword;
};
struct This : Keyword {
    using Keyword::Keyword;
};
struct True : Keyword {
    using Keyword::Keyword;
};
struct Var : Keyword {
    using Keyword::Keyword;
};
struct While : Keyword {
    using Keyword::Keyword;
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
};

const char *getName(const Token &token);
std::ostream &operator<<(std::ostream &strm, const Token &a);
} // namespace Token
#endif