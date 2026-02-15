#include "lexer/lexer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>

// Simple test macros
static int test_failures = 0;
static int test_assertions = 0;

#define ASSERT_EQ(expected, actual) do { \
    test_assertions++; \
    if ((expected) != (actual)) { \
        std::cerr << "  FAIL at line " << __LINE__ << ": expected '" << (expected) \
                  << "' but got '" << (actual) << "'" << std::endl; \
        test_failures++; \
    } \
} while(0)

// ---- Test cases ----

void test_keywords() {
    Lexer lexer("fn let mut if else while return");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(8u, tokens.size()); // 7 keywords + EOF
    ASSERT_EQ(TokenType::KW_FN,     tokens[0].type);
    ASSERT_EQ(TokenType::KW_LET,    tokens[1].type);
    ASSERT_EQ(TokenType::KW_MUT,    tokens[2].type);
    ASSERT_EQ(TokenType::KW_IF,     tokens[3].type);
    ASSERT_EQ(TokenType::KW_ELSE,   tokens[4].type);
    ASSERT_EQ(TokenType::KW_WHILE,  tokens[5].type);
    ASSERT_EQ(TokenType::KW_RETURN, tokens[6].type);
    ASSERT_EQ(TokenType::END_OF_FILE, tokens[7].type);
}

void test_identifiers() {
    Lexer lexer("foo _bar x1");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(4u, tokens.size()); // 3 identifiers + EOF
    ASSERT_EQ(TokenType::IDENTIFIER, tokens[0].type);
    ASSERT_EQ(std::string("foo"),    tokens[0].lexeme);
    ASSERT_EQ(TokenType::IDENTIFIER, tokens[1].type);
    ASSERT_EQ(std::string("_bar"),   tokens[1].lexeme);
    ASSERT_EQ(TokenType::IDENTIFIER, tokens[2].type);
    ASSERT_EQ(std::string("x1"),     tokens[2].lexeme);
}

void test_numbers() {
    Lexer lexer("42 0");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(3u, tokens.size()); // 2 numbers + EOF
    ASSERT_EQ(TokenType::INTEGER, tokens[0].type);
    ASSERT_EQ(std::string("42"), tokens[0].lexeme);
    ASSERT_EQ(TokenType::INTEGER, tokens[1].type);
    ASSERT_EQ(std::string("0"),  tokens[1].lexeme);
}

void test_strings() {
    Lexer lexer("\"hello\"");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(2u, tokens.size()); // 1 string + EOF
    ASSERT_EQ(TokenType::STRING, tokens[0].type);
    ASSERT_EQ(std::string("hello"), tokens[0].lexeme);
}

void test_operators() {
    Lexer lexer("+ - * / = == != < > <= >=");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(12u, tokens.size()); // 11 operators + EOF
    ASSERT_EQ(TokenType::PLUS,   tokens[0].type);
    ASSERT_EQ(TokenType::MINUS,  tokens[1].type);
    ASSERT_EQ(TokenType::STAR,   tokens[2].type);
    ASSERT_EQ(TokenType::SLASH,  tokens[3].type);
    ASSERT_EQ(TokenType::ASSIGN, tokens[4].type);
    ASSERT_EQ(TokenType::EQ,     tokens[5].type);
    ASSERT_EQ(TokenType::NEQ,    tokens[6].type);
    ASSERT_EQ(TokenType::LT,     tokens[7].type);
    ASSERT_EQ(TokenType::GT,     tokens[8].type);
    ASSERT_EQ(TokenType::LTE,    tokens[9].type);
    ASSERT_EQ(TokenType::GTE,    tokens[10].type);
}

void test_punctuation() {
    Lexer lexer("( ) { } ; : ,");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(8u, tokens.size()); // 7 punctuation + EOF
    ASSERT_EQ(TokenType::LPAREN,    tokens[0].type);
    ASSERT_EQ(TokenType::RPAREN,    tokens[1].type);
    ASSERT_EQ(TokenType::LBRACE,    tokens[2].type);
    ASSERT_EQ(TokenType::RBRACE,    tokens[3].type);
    ASSERT_EQ(TokenType::SEMICOLON, tokens[4].type);
    ASSERT_EQ(TokenType::COLON,     tokens[5].type);
    ASSERT_EQ(TokenType::COMMA,     tokens[6].type);
}

void test_whitespace_comments() {
    // Whitespace is skipped
    Lexer lexer1("  \t  \n  x  ");
    auto t1 = lexer1.tokenize();
    ASSERT_EQ(2u, t1.size()); // x + EOF
    ASSERT_EQ(TokenType::IDENTIFIER, t1[0].type);
    ASSERT_EQ(std::string("x"), t1[0].lexeme);

    // Line comments are skipped
    Lexer lexer2("x // this is a comment\ny");
    auto t2 = lexer2.tokenize();
    ASSERT_EQ(3u, t2.size()); // x, y, EOF
    ASSERT_EQ(TokenType::IDENTIFIER, t2[0].type);
    ASSERT_EQ(std::string("x"), t2[0].lexeme);
    ASSERT_EQ(TokenType::IDENTIFIER, t2[1].type);
    ASSERT_EQ(std::string("y"), t2[1].lexeme);
}

void test_edge_fn_name() {
    // fn_name should be IDENTIFIER, not KW_FN
    Lexer lexer("fn_name");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(2u, tokens.size()); // 1 identifier + EOF
    ASSERT_EQ(TokenType::IDENTIFIER, tokens[0].type);
    ASSERT_EQ(std::string("fn_name"), tokens[0].lexeme);
}

void test_edge_eq() {
    // == should be EQ, not two ASSIGNs
    Lexer lexer("==");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(2u, tokens.size()); // 1 EQ + EOF
    ASSERT_EQ(TokenType::EQ, tokens[0].type);
    ASSERT_EQ(std::string("=="), tokens[0].lexeme);
}

// ---- Test runner ----

struct TestEntry {
    const char* name;
    void (*func)();
};

static TestEntry all_tests[] = {
    {"keywords",            test_keywords},
    {"identifiers",         test_identifiers},
    {"numbers",             test_numbers},
    {"strings",             test_strings},
    {"operators",           test_operators},
    {"punctuation",         test_punctuation},
    {"whitespace_comments", test_whitespace_comments},
    {"edge_fn_name",        test_edge_fn_name},
    {"edge_eq",             test_edge_eq},
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: test_lexer <test_name>" << std::endl;
        std::cerr << "Available tests:" << std::endl;
        for (auto& t : all_tests) {
            std::cerr << "  " << t.name << std::endl;
        }
        return 1;
    }

    const char* target = argv[1];
    for (auto& t : all_tests) {
        if (std::strcmp(t.name, target) == 0) {
            test_failures = 0;
            test_assertions = 0;
            t.func();
            if (test_failures == 0) {
                std::cout << "PASS: " << t.name << " (" << test_assertions << " assertions)" << std::endl;
                return 0;
            } else {
                std::cerr << "FAIL: " << t.name << " (" << test_failures << " failures out of "
                          << test_assertions << " assertions)" << std::endl;
                return 1;
            }
        }
    }

    std::cerr << "Unknown test: " << target << std::endl;
    return 1;
}
