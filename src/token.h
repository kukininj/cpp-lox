#include "position.h"
#include <iostream>
#include <variant>
#include <optional>

#ifndef TOKEN
#define TOKEN


enum class TokenType {
    // clang-format off
    // Single-character tokens.
    LeftParen, RightParen, LeftBrace, RightBrace,
    Comma, Dot, Minus, Plus, Semicolon, Slash, Star,

    // One or two character tokens.
    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,

    // Literals.
    Identifier, String, Number,

    // Keywords.
    And, Class, Else, False, Fun, For, If, Nil, Or,
    Print, Return, Super, This, True, Var, While,

    Eof
    // clang-format on
};

namespace Token {
typedef std::variant<std::string, double> Literal;

struct Token {
    TokenType type;
    std::string lexeme;
    std::optional<Literal> literal;
    Position position;
};

std::ostream &operator<<(std::ostream &strm, const Token &a); 
std::ostream& operator<<(std::ostream &stream, const Literal &t);
}

std::ostream& operator<<(std::ostream &stream, const TokenType &t);
#endif