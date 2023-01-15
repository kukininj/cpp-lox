#include "scanner.h"
#include "exceptions.h"
#include "token.h"
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>
#include <functional>

bool Scanner::isAtEnd() { return this->sourcePosition >= this->source.size(); }

char Scanner::currentCharacter() {
    if (this->isAtEnd())
        return '\0';

    return source[sourcePosition];
}

Position Scanner::getPosition() const {
    return Position{
        .line = this->lineNumber,
        .position = this->linePosition,
    };
}

void Scanner::advanceLinePosition() {
    size_t len = this->sourcePosition - this->lexemeStart;
    this->linePosition += len - 1;
}

std::string Scanner::getLexeme() const {
    size_t len = this->sourcePosition - this->lexemeStart;
    return this->source.substr(this->lexemeStart, len);
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

    this->tokens.push_back(Token::Eof("", getPosition()));

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
    case '(': this->addToken<Token::LeftParen>(); break;
    case ')': this->addToken<Token::RightParen>(); break;
    case '{': this->addToken<Token::LeftBrace>(); break;
    case '}': this->addToken<Token::RightBrace>(); break;
    case ',': this->addToken<Token::Comma>(); break;
    case '.': this->addToken<Token::Dot>(); break;
    case '-': this->addToken<Token::Minus>(); break;
    case '+': this->addToken<Token::Plus>(); break;
    case ';': this->addToken<Token::Semicolon>(); break;
    case '*': this->addToken<Token::Star>(); break;
    // clang-format on
    case '!':
        this->match('=') ? this->addToken<Token::BangEqual>()
                         : this->addToken<Token::Bang>();
        break;
    case '=':
        this->match('=') ? this->addToken<Token::EqualEqual>()
                         : this->addToken<Token::Equal>();
        break;
    case '<':
        this->match('=') ? this->addToken<Token::LessEqual>()
                         : this->addToken<Token::Less>();
        break;
    case '>':
        this->match('=') ? this->addToken<Token::GreaterEqual>()
                         : this->addToken<Token::Greater>();
        break;
    case '/':
        if (this->match('/'))
            this->skipComment();
        else
            this->addToken<Token::Slash>();
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

    static std::unordered_map<std::string,
                              std::function<Token::Token (Scanner & s)>>
        keywords{
            {"and", &createKeyword<Token::And>},
            {"class", &createKeyword<Token::Class>},
            {"else", &createKeyword<Token::Else>},
            {"false", &createKeyword<Token::False>},
            {"fun", &createKeyword<Token::Fun>},
            {"for", &createKeyword<Token::For>},
            {"if", &createKeyword<Token::If>},
            {"nil", &createKeyword<Token::Nil>},
            {"or", &createKeyword<Token::Or>},
            {"print", &createKeyword<Token::Print>},
            {"return", &createKeyword<Token::Return>},
            {"super", &createKeyword<Token::Super>},
            {"this", &createKeyword<Token::This>},
            {"true", &createKeyword<Token::True>},
            {"var", &createKeyword<Token::Var>},
            {"while", &createKeyword<Token::While>},
        };

    if (auto type = keywords.find(name); type != keywords.end()) {
        this->tokens.push_back(type->second(*this));
    } else {
        this->addIdentifier(name);
    }
}

template <typename... Ts> struct Overload : Ts... { using Ts::operator()...; };
template <class... Ts> Overload(Ts...) -> Overload<Ts...>;
void Scanner::addToken(Token::Literal literal) {
    std::string lexeme = this->getLexeme();

    auto toke = std::visit(Overload{
        [&](std::string &value) -> Token::Token {
            Token::Token t(*(new Token::StringLiteral(value, lexeme, this->getPosition())));
            return t;
        } , [&](double &value) -> Token::Token {
            Token::Token t(*(new Token::NumberLiteral(value, lexeme, this->getPosition())));
            return t;
        }
}, literal);

    this->tokens.push_back(toke);
    this->advanceLinePosition();
}

void Scanner::addIdentifier(std::string name) {
    size_t len = this->sourcePosition - this->lexemeStart;
    std::string lexeme = this->source.substr(this->lexemeStart, len);

    this->tokens.push_back(Token::Identifier { name, lexeme, this->getPosition(),
    });
    this->advanceLinePosition();
}
