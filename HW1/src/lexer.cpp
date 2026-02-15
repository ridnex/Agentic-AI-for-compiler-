#include "lexer/lexer.h"
#include <unordered_map>

static const std::unordered_map<std::string, TokenType> keywords = {
    {"fn",     TokenType::KW_FN},
    {"let",    TokenType::KW_LET},
    {"mut",    TokenType::KW_MUT},
    {"if",     TokenType::KW_IF},
    {"else",   TokenType::KW_ELSE},
    {"while",  TokenType::KW_WHILE},
    {"return", TokenType::KW_RETURN},
};

Lexer::Lexer(const std::string& source)
    : source_(source), pos_(0), line_(1), column_(1) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;
        tokens.push_back(scanToken());
    }
    tokens.push_back(makeToken(TokenType::END_OF_FILE, "", line_, column_));
    return tokens;
}

bool Lexer::isAtEnd() const {
    return pos_ >= source_.size();
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[pos_];
}

char Lexer::peekNext() const {
    if (pos_ + 1 >= source_.size()) return '\0';
    return source_[pos_ + 1];
}

char Lexer::advance() {
    char c = source_[pos_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source_[pos_] != expected) return false;
    advance();
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else if (c == '/' && peekNext() == '/') {
            // Line comment — consume until end of line
            advance(); // '/'
            advance(); // '/'
            while (!isAtEnd() && peek() != '\n') {
                advance();
            }
        } else {
            break;
        }
    }
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme, int startLine, int startCol) {
    return Token{type, lexeme, startLine, startCol};
}

Token Lexer::scanToken() {
    int startLine = line_;
    int startCol = column_;
    char c = advance();

    switch (c) {
        // Punctuation
        case '(': return makeToken(TokenType::LPAREN,    "(", startLine, startCol);
        case ')': return makeToken(TokenType::RPAREN,    ")", startLine, startCol);
        case '{': return makeToken(TokenType::LBRACE,    "{", startLine, startCol);
        case '}': return makeToken(TokenType::RBRACE,    "}", startLine, startCol);
        case ';': return makeToken(TokenType::SEMICOLON, ";", startLine, startCol);
        case ':': return makeToken(TokenType::COLON,     ":", startLine, startCol);
        case ',': return makeToken(TokenType::COMMA,     ",", startLine, startCol);

        // Single-char operators
        case '+': return makeToken(TokenType::PLUS,  "+", startLine, startCol);
        case '-': return makeToken(TokenType::MINUS, "-", startLine, startCol);
        case '*': return makeToken(TokenType::STAR,  "*", startLine, startCol);
        case '/': return makeToken(TokenType::SLASH, "/", startLine, startCol);

        // Two-char operators
        case '=':
            if (match('=')) return makeToken(TokenType::EQ,  "==", startLine, startCol);
            return makeToken(TokenType::ASSIGN, "=", startLine, startCol);
        case '!':
            if (match('=')) return makeToken(TokenType::NEQ, "!=", startLine, startCol);
            return makeToken(TokenType::ERROR, "!", startLine, startCol);
        case '<':
            if (match('=')) return makeToken(TokenType::LTE, "<=", startLine, startCol);
            return makeToken(TokenType::LT, "<", startLine, startCol);
        case '>':
            if (match('=')) return makeToken(TokenType::GTE, ">=", startLine, startCol);
            return makeToken(TokenType::GT, ">", startLine, startCol);

        // String literals
        case '"': return scanString();

        default:
            // Identifiers and keywords
            if (std::isalpha(c) || c == '_') {
                return scanIdentifierOrKeyword();
            }
            // Numbers
            if (std::isdigit(c)) {
                return scanNumber();
            }
            // Unknown character — emit ERROR token
            return makeToken(TokenType::ERROR, std::string(1, c), startLine, startCol);
    }
}

Token Lexer::scanIdentifierOrKeyword() {
    // The first character was already consumed by scanToken
    int startLine = line_;
    int startCol = column_ - 1; // adjust since first char already advanced column
    // Handle line boundary: if advance() crossed a newline, startCol was captured in scanToken
    // Actually, scanToken captured startLine/startCol before advance(), but we don't have access here.
    // Let's reconstruct: the first char started at (column_ - 1) on the current line.

    size_t start = pos_ - 1; // position of first char in source_

    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }

    std::string lexeme = source_.substr(start, pos_ - start);

    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return makeToken(it->second, lexeme, startLine, startCol);
    }
    return makeToken(TokenType::IDENTIFIER, lexeme, startLine, startCol);
}

Token Lexer::scanNumber() {
    int startLine = line_;
    int startCol = column_ - 1;
    size_t start = pos_ - 1;

    while (!isAtEnd() && std::isdigit(peek())) {
        advance();
    }

    std::string lexeme = source_.substr(start, pos_ - start);
    return makeToken(TokenType::INTEGER, lexeme, startLine, startCol);
}

Token Lexer::scanString() {
    // Opening '"' was already consumed by scanToken
    int startLine = line_;
    int startCol = column_ - 1;
    std::string value;

    while (!isAtEnd() && peek() != '"') {
        value += advance();
    }

    if (isAtEnd()) {
        // Unterminated string
        return makeToken(TokenType::ERROR, "\"" + value, startLine, startCol);
    }

    advance(); // consume closing '"'
    return makeToken(TokenType::STRING, value, startLine, startCol);
}
