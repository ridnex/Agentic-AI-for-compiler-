#pragma once

#include <string>
#include <ostream>

enum class TokenType {
    // Keywords
    KW_FN,
    KW_LET,
    KW_MUT,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_RETURN,

    // Literals
    IDENTIFIER,
    INTEGER,
    STRING,

    // Operators
    PLUS,       // +
    MINUS,      // -
    STAR,       // *
    SLASH,      // /
    ASSIGN,     // =
    EQ,         // ==
    NEQ,        // !=
    LT,         // <
    GT,         // >
    LTE,        // <=
    GTE,        // >=

    // Punctuation
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    SEMICOLON,  // ;
    COLON,      // :
    COMMA,      // ,

    // Special
    END_OF_FILE,
    ERROR
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KW_FN:        return "KW_FN";
        case TokenType::KW_LET:       return "KW_LET";
        case TokenType::KW_MUT:       return "KW_MUT";
        case TokenType::KW_IF:        return "KW_IF";
        case TokenType::KW_ELSE:      return "KW_ELSE";
        case TokenType::KW_WHILE:     return "KW_WHILE";
        case TokenType::KW_RETURN:    return "KW_RETURN";
        case TokenType::IDENTIFIER:   return "IDENTIFIER";
        case TokenType::INTEGER:      return "INTEGER";
        case TokenType::STRING:       return "STRING";
        case TokenType::PLUS:         return "PLUS";
        case TokenType::MINUS:        return "MINUS";
        case TokenType::STAR:         return "STAR";
        case TokenType::SLASH:        return "SLASH";
        case TokenType::ASSIGN:       return "ASSIGN";
        case TokenType::EQ:           return "EQ";
        case TokenType::NEQ:          return "NEQ";
        case TokenType::LT:           return "LT";
        case TokenType::GT:           return "GT";
        case TokenType::LTE:          return "LTE";
        case TokenType::GTE:          return "GTE";
        case TokenType::LPAREN:       return "LPAREN";
        case TokenType::RPAREN:       return "RPAREN";
        case TokenType::LBRACE:       return "LBRACE";
        case TokenType::RBRACE:       return "RBRACE";
        case TokenType::SEMICOLON:    return "SEMICOLON";
        case TokenType::COLON:        return "COLON";
        case TokenType::COMMA:        return "COMMA";
        case TokenType::END_OF_FILE:  return "EOF";
        case TokenType::ERROR:        return "ERROR";
    }
    return "UNKNOWN";
}

inline std::ostream& operator<<(std::ostream& os, TokenType type) {
    os << tokenTypeToString(type);
    return os;
}

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

inline std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.line << ":" << token.column << "  "
       << tokenTypeToString(token.type) << "  " << token.lexeme;
    return os;
}
