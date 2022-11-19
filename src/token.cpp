#include "token.h"
#include <iostream>
#include <variant>

template <typename... Ts> struct Overload : Ts... { using Ts::operator()...; };
template <class... Ts> Overload(Ts...) -> Overload<Ts...>;

namespace Token {
std::ostream &operator<<(std::ostream &strm, const Token &a) {
    if (a.literal.has_value()) {
        return strm << "Token: { "
                    << "type: " << a.type << ", lexeme: \"" << a.lexeme
                    << "\", "
                    << "value: " << a.literal.value() << ", "
                    << "position: {" << a.position << "} }";
    } else {
        return strm << "Token: { "
                    << "type: " << a.type << ", lexeme: \"" << a.lexeme
                    << "\", "
                    << "position: {" << a.position << "} }";
    }
}

std::ostream &operator<<(std::ostream &strm, const Literal &l) {
    return std::visit(
        Overload{
            [&strm](const auto &s) -> std::ostream & { return strm << s; }
            // [&strm](const std::string &s) -> std::ostream & {
            //     return strm << s;
            // }
            // [&strm](const double &d) -> std::ostream & {
            //     return strm << d;
            // }
        },
        l);
}
} // namespace Token

std::ostream &operator<<(std::ostream &strm, const TokenType &a) {
    switch (a) {
    case TokenType::LeftParen:
        return strm << "LeftParen";
    case TokenType::RightParen:
        return strm << "RightParen";
    case TokenType::LeftBrace:
        return strm << "LeftBrace";
    case TokenType::RightBrace:
        return strm << "RightBrace";
    case TokenType::Comma:
        return strm << "Comma";
    case TokenType::Dot:
        return strm << "Dot";
    case TokenType::Minus:
        return strm << "Minus";
    case TokenType::Plus:
        return strm << "Plus";
    case TokenType::Semicolon:
        return strm << "Semicolon";
    case TokenType::Slash:
        return strm << "Slash";
    case TokenType::Star:
        return strm << "Star";
    case TokenType::Bang:
        return strm << "Bang";
    case TokenType::BangEqual:
        return strm << "BangEqual";
    case TokenType::Equal:
        return strm << "Equal";
    case TokenType::EqualEqual:
        return strm << "EqualEqual";
    case TokenType::Greater:
        return strm << "Greater";
    case TokenType::GreaterEqual:
        return strm << "GreaterEqual";
    case TokenType::Less:
        return strm << "Less";
    case TokenType::LessEqual:
        return strm << "LessEqual";
    case TokenType::Identifier:
        return strm << "Identifier";
    case TokenType::String:
        return strm << "String";
    case TokenType::Number:
        return strm << "Number";
    case TokenType::And:
        return strm << "And";
    case TokenType::Class:
        return strm << "Class";
    case TokenType::Else:
        return strm << "Else";
    case TokenType::False:
        return strm << "False";
    case TokenType::Fun:
        return strm << "Fun";
    case TokenType::For:
        return strm << "For";
    case TokenType::If:
        return strm << "If";
    case TokenType::Nil:
        return strm << "Nil";
    case TokenType::Or:
        return strm << "Or";
    case TokenType::Print:
        return strm << "Print";
    case TokenType::Return:
        return strm << "Return";
    case TokenType::Super:
        return strm << "Super";
    case TokenType::This:
        return strm << "This";
    case TokenType::True:
        return strm << "True";
    case TokenType::Var:
        return strm << "Var";
    case TokenType::While:
        return strm << "While";
    case TokenType::Eof:
        return strm << "Eof";
        break;
    }
}
