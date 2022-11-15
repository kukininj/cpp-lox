#include "scanner.h"
#include "exceptions.h"
#include "token.h"
#include <vector>

bool Scanner::isAtEnd() { return this->sourcePosition >= this->source.size(); }

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
    if (this->source[this->sourcePosition] != c)
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
        switch (this->source[this->sourcePosition]) {
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

void Scanner::addToken(TokenType type) {
    size_t len = this->sourcePosition - this->lexemeStart;
    std::string lexeme =
        this->source.substr(this->lexemeStart, len);

    switch (type) {
    case TokenType::Identifier:
        throw new Exceptions::NotImplemented(
            "Identifiers are not implemented!");
        break;
    case TokenType::String:
    case TokenType::Number:
        throw new Exceptions::NotImplemented("Literals are not implemented!");
        break;
    default:
        this->tokens.push_back(Token::Token{.type = type,
                                            .lexeme = lexeme,
                                            .position = Position{
                                                .line = this->lineNumber,
                                                .position = this->linePosition,
                                            }});
        break;
    }
    this->linePosition += len - 1;
}