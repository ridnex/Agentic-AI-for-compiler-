#pragma once

#include "lexer/token.h"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(const std::string& source);

    std::vector<Token> tokenize();

private:
    std::string source_;
    size_t pos_;
    int line_;
    int column_;

    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();
    bool match(char expected);

    void skipWhitespace();
    Token scanToken();
    Token scanIdentifierOrKeyword();
    Token scanNumber();
    Token scanString();

    Token makeToken(TokenType type, const std::string& lexeme, int startLine, int startCol);
};
