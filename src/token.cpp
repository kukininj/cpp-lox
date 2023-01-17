#include "token.h"
#include <iostream>
#include <variant>

template <typename... Ts> struct Overload : Ts... { using Ts::operator()...; };
template <class... Ts> Overload(Ts...) -> Overload<Ts...>;

namespace Token {
std::ostream &operator<<(std::ostream &strm, const Token &token) {
    return std::visit(
        Overload{[&strm](const StringLiteral &token) -> std::ostream & {
                     return strm << "Token:{"
                                 << " type: " << getName(token) // 
                                 // << " lexeme: \"" << token.lexeme << "\""
                                 << " value: " << token.value << " pos:{"
                                 << token.position << "} }";
                 },
                 [&strm](const NumberLiteral &token) -> std::ostream & {
                     return strm << "Token:{"
                                 << " type: " << getName(token) //
                                 // << " lexeme: \"" << token.lexeme << "\""
                                 << " value: " << token.value << " pos:{"
                                 << token.position << "} }";
                 },
                 [&strm](const Identifier &token) -> std::ostream & {
                     return strm << "Token:{"
                                 << " type: " << getName(token) //
                                 // << " lexeme: \"" << token.lexeme << "\""
                                 << " value: " << token.value << " pos:{"
                                 << token.position << "} }";
                 },
                 [&strm](const auto &token) -> std::ostream & {
                     return strm << "Token:{"
                                 << " type: " << getName(token) // 
                                 // << " lexeme: \"" << token.lexeme << "\""
                                 << " pos:{" << token.position << "} }";
                 }

        },
        token);
}

const char *getName(const Token &token) {
    // [](const Operator &t) -> auto{ return "Operator"; },
    // [](const Special &t) -> auto{ return "Special"; },
    // [](const Keyword &t) -> auto{ return "Keyword"; },
    // clang-format off
    return std::visit(
    Overload{ 
      [](const Minus &t) -> auto {return "Minus"; }
    , [](const Plus &t) -> auto{ return "Plus"; }
    , [](const Semicolon &t) -> auto{ return "Semicolon"; }
    , [](const Slash &t) -> auto{ return "Slash"; }
    , [](const Star &t) -> auto{ return "Star"; }
    , [](const Bang &t) -> auto{ return "Bang"; }
    , [](const BangEqual &t) -> auto{ return "BangEqual"; }
    , [](const Equal &t) -> auto{ return "Equal"; }
    , [](const EqualEqual &t) -> auto{ return "EqualEqual"; }
    , [](const Greater &t) -> auto{ return "Greater"; }
    , [](const GreaterEqual &t) -> auto{ return "GreaterEqual"; }
    , [](const Less &t) -> auto{ return "Less"; }
    , [](const LessEqual &t) -> auto{ return "LessEqual"; }
    , [](const LeftParen &t) -> auto{ return "LeftParen"; }
    , [](const RightParen &t) -> auto{ return "RightParen"; }
    , [](const LeftBrace &t) -> auto{ return "LeftBrace"; }
    , [](const RightBrace &t) -> auto{ return "RightBrace"; }
    , [](const Comma &t) -> auto{ return "Comma"; }
    , [](const Dot &t) -> auto{ return "Dot"; }
    , [](const Eof &t) -> auto{ return "Eof"; }
    , [](const And &t) -> auto{ return "And"; }
    , [](const Class &t) -> auto{ return "Class"; }
    , [](const Else &t) -> auto{ return "Else"; }
    , [](const False &t) -> auto{ return "False"; }
    , [](const Fun &t) -> auto{ return "Fun"; }
    , [](const For &t) -> auto{ return "For"; }
    , [](const If &t) -> auto{ return "If"; }
    , [](const Nil &t) -> auto{ return "Nil"; }
    , [](const Or &t) -> auto{ return "Or"; }
    , [](const Print &t) -> auto{ return "Print"; }
    , [](const Return &t) -> auto{ return "Return"; }
    , [](const Super &t) -> auto{ return "Super"; }
    , [](const This &t) -> auto{ return "This"; }
    , [](const True &t) -> auto{ return "True"; }
    , [](const Var &t) -> auto{ return "Var"; }
    , [](const While &t) -> auto{ return "While"; }
    , [](const Identifier &t) -> auto{ return "Identifier"; }
    , [](const StringLiteral &t) -> auto{ return "StringLiteral"; }
    , [](const NumberLiteral &t) -> auto{ return "NumberLiteral"; }
    }, token);
// clang-format on
}

} // namespace Token
