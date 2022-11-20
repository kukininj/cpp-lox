#include "scanner.h"
#include "exceptions.h"
#include "token.h"
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>

bool Scanner::isAtEnd() { return this->sourcePosition >= this->source.size(); }

char Scanner::currentCharacter() {
    if (this->isAtEnd())
        return '\0';

    return source[sourcePosition];
}

Position Scanner::getPosition() {
    return Position{
        .line = this->lineNumber,
        .position = this->linePosition,
    };
}

char Scanner::nextCharacter() {
    if (this->isAtEnd())
        return '\0';

    return source[sourcePosition + 1];
}
Scanner::Scanner(std::string source) {
    lineNumber = 1;
    linePosition = 0;
    lexemeStart = 0;
    sourcePosition = 0;
    this->source = source;
}

std::vector<Token::Token> Scanner::getTokens() {
    while (!isAtEnd()) {
        this->lexemeStart = this->sourcePosition;
        this->scanToken();
    }

    return this->tokens;
}

char Scanner::consumeCharacter() {
    this->linePosition++;
    return this->source[this->sourcePosition++];
}

bool Scanner::match(char c) {
    if (isAtEnd())
        return false;
    if (this->currentCharacter() != c)
        return false;

    this->sourcePosition++;
    // this->linePosition++;
    return true;
}

void Scanner::scanToken() {
    this->lexemeStart = this->sourcePosition;
    char c = this->consumeCharacter();
    switch (c) {
    // clang-format off
    case '(': this->addToken(TokenType::LeftParen); break;
    case ')': this->addToken(TokenType::RightParen); break;
    case '{': this->addToken(TokenType::LeftBrace); break;
    case '}': this->addToken(TokenType::RightBrace); break;
    case ',': this->addToken(TokenType::Comma); break;
    case '.': this->addToken(TokenType::Dot); break;
    case '-': this->addToken(TokenType::Minus); break;
    case '+': this->addToken(TokenType::Plus); break;
    case ';': this->addToken(TokenType::Semicolon); break;
    case '*': this->addToken(TokenType::Star); break;
    // clang-format on
    case '!':
        this->addToken(this->match('=') ? TokenType::BangEqual
                                        : TokenType::Bang);
        break;
    case '=':
        this->addToken(this->match('=') ? TokenType::EqualEqual
                                        : TokenType::Equal);
        break;
    case '<':
        this->addToken(this->match('=') ? TokenType::LessEqual
                                        : TokenType::Less);
        break;
    case '>':
        this->addToken(this->match('=') ? TokenType::GreaterEqual
                                        : TokenType::Greater);
        break;
    case '/':
        if (this->match('/'))
            this->skipComment();
        else
            this->addToken(TokenType::Slash);
        break;
    case ' ':
    case '\t':
    case '\r':
        this->skipWhitespaceCharacters();
        break;
    case '\n':
        this->lineNumber++;
        this->linePosition = 0;
        break;
    case '"':
        this->scanStringLiteral();
        break;
    case '0' ... '9':
        this->scanNumberLiteral();
        break;
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
        this->scanIdentifier();
        break;
    default:
        throw new Exceptions::SyntaxError("Unexpected character at");
        break;
    }
}

void Scanner::skipComment() {
    // what happens when we reach end of file?
    size_t commentEnd = this->source.find('\n', this->sourcePosition);
    if (commentEnd == std::string::npos)
        throw std::logic_error("Expected end_of_line after comment");

    // size_t commentLength = commentEnd - this->sourcePosition;
    this->sourcePosition = commentEnd + 1;
    this->linePosition = 0;
    this->lineNumber += 1;
}

void Scanner::skipWhitespaceCharacters() {
    while (this->sourcePosition < this->source.size()) {
        switch (this->currentCharacter()) {
        case ' ':
        case '\t':
        case '\r':
            this->sourcePosition++;
            // this->linePosition++;
            break;
        case '\n':
            this->sourcePosition++;
            this->linePosition = 0;
            this->lineNumber++;
            break;
        default:
            return;
        }
    }
}

void Scanner::scanStringLiteral() {
    while ((this->currentCharacter() != '"') && !this->isAtEnd()) {
        this->sourcePosition++;
        // this->currentCharacter() may only be '"' of '\n'
        if (this->isAtEnd()) {
            throw new Exceptions::SyntaxError("Unterminated string literal");
        }
    }

    // skip ending '"'
    this->sourcePosition++;

    size_t len = this->sourcePosition - this->lexemeStart - 1;
    std::string value = this->source.substr(this->lexemeStart + 1, len - 1);
    this->addToken(value);
}

void Scanner::scanNumberLiteral() {
    while ((std::isdigit(this->currentCharacter()))) {
        this->sourcePosition++;
    }

    if (this->currentCharacter() == '.' &&
        std::isdigit(this->nextCharacter())) {
        this->sourcePosition++;

        while ((std::isdigit(this->currentCharacter()))) {
            this->sourcePosition++;
        }
    }

    auto len = this->sourcePosition - this->lexemeStart;
    auto number = source.substr(this->lexemeStart, len);
    double value = std::stof(number);
    this->addToken(value);
}

bool isAlphaNumeric(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}
void Scanner::scanIdentifier() {
    while ((isAlphaNumeric(this->currentCharacter())) && !this->isAtEnd()) {
        this->sourcePosition++;
        // this->currentCharacter() may only be '"' of '\n'
    }

    size_t len = this->sourcePosition - this->lexemeStart;
    std::string name = this->source.substr(this->lexemeStart, len);

    static std::unordered_map<std::string, TokenType> keywords{
        {"and", TokenType::And},       {"class", TokenType::Class},
        {"else", TokenType::Else},     {"false", TokenType::False},
        {"for", TokenType::For},       {"fun", TokenType::Fun},
        {"if", TokenType::If},         {"nil", TokenType::Nil},
        {"or", TokenType::Or},         {"print", TokenType::Print},
        {"return", TokenType::Return}, {"super", TokenType::Super},
        {"this", TokenType::This},     {"true", TokenType::True},
        {"var", TokenType::Var},       {"while", TokenType::While},
    };

    if (auto type = keywords.find(name); type != keywords.end()) {
        this->addToken(type->second);
    } else {
        this->addIdentifier(name);
    }
}

void Scanner::addToken(TokenType type) {
    size_t len = this->sourcePosition - this->lexemeStart;
    std::string lexeme = this->source.substr(this->lexemeStart, len);

    switch (type) {
    case TokenType::Identifier:
        throw new std::logic_error("This function can't handle Identifiers!");
    case TokenType::String:
        throw new std::logic_error(
            "This function can't handle String literals!");
    case TokenType::Number:
        throw new std::logic_error(
            "This function can't handle Number literals!");
    default:
        this->tokens.push_back(Token::Token{
            .type = type, .lexeme = lexeme, .position = this->getPosition()});
        break;
    }
    this->linePosition += len - 1;
}

template <typename... Ts> struct Overload : Ts... { using Ts::operator()...; };
template <class... Ts> Overload(Ts...) -> Overload<Ts...>;
void Scanner::addToken(Token::Literal literal) {
    size_t len = this->sourcePosition - this->lexemeStart;
    std::string lexeme = this->source.substr(this->lexemeStart, len);

    TokenType type = TokenType::Eof;

    std::visit(Overload{[&type](std::string &_) { type = TokenType::String; },
                        [&type](double &_) { type = TokenType::Number; }},
               literal);

    this->tokens.push_back(Token::Token{
        .type = type,
        .lexeme = lexeme,
        .literal = literal,
        .position = this->getPosition(),
    });
    this->linePosition += len - 1;
}

void Scanner::addIdentifier(std::string name) {
    size_t len = this->sourcePosition - this->lexemeStart;
    std::string lexeme = this->source.substr(this->lexemeStart, len);

    this->tokens.push_back(Token::Token{
        .type = TokenType::Identifier,
        .lexeme = lexeme,
        .literal = name,
        .position = this->getPosition(),
    });
    this->linePosition += len - 1;
}
