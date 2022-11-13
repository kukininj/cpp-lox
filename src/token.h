#include "position.h"
#include <iostream>
#include <variant>

#ifndef TOKEN
#define TOKEN


namespace Token {
enum class TokenType {
    // clang-format off
    // Single-character tokens.
    LeftParen, RightParen, leftBrace, RightBrace,
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

std::ostream& operator<<(std::ostream &stream, const TokenType &t);

struct Token {
    TokenType type;
    std::string lexeme;
    std::variant<std::string, double> literal;
    Position position;
};

std::ostream &operator<<(std::ostream &strm, const Token &a); 
}
#endif