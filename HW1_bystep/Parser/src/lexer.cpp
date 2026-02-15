#include "lexer.h"

std::vector<Token> Lexer::tokenize(const std::string& source) {
    std::vector<Token> tokens;
    int pos = 0;
    int length = source.length();

    while (pos < length) {
        char ch = source[pos];

        // Skip whitespace
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
            pos++;
            continue;
        }

        // Read a word (letters, digits, underscores)
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            std::string word;
            while (pos < length) {
                char c = source[pos];
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                    (c >= '0' && c <= '9') || c == '_') {
                    word += c;
                    pos++;
                } else {
                    break;
                }
            }

            // Check if it's a keyword
            if (word == "fn" || word == "let" || word == "mut" ||
                word == "if" || word == "else" || word == "while" ||
                word == "return") {
                tokens.push_back({"KEYWORD", word});
            } else {
                tokens.push_back({"IDENTIFIER", word});
            }
            continue;
        }

        // Read a number
        if (ch >= '0' && ch <= '9') {
            std::string number;
            while (pos < length && source[pos] >= '0' && source[pos] <= '9') {
                number += source[pos];
                pos++;
            }
            tokens.push_back({"NUMBER", number});
            continue;
        }

        // Read a string
        if (ch == '"') {
            std::string str;
            pos++; // skip opening "
            while (pos < length && source[pos] != '"') {
                str += source[pos];
                pos++;
            }
            pos++; // skip closing "
            tokens.push_back({"STRING", str});
            continue;
        }

        // Skip comments (// until end of line)
        if (ch == '/' && pos + 1 < length && source[pos + 1] == '/') {
            while (pos < length && source[pos] != '\n') {
                pos++;
            }
            continue;
        }

        // Two-char operators: ==, !=, <=, >=
        if (pos + 1 < length && source[pos + 1] == '=') {
            if (ch == '=' || ch == '!' || ch == '<' || ch == '>') {
                std::string op;
                op += ch;
                op += '=';
                tokens.push_back({"OPERATOR", op});
                pos += 2;
                continue;
            }
        }

        // Single-char operators
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '=' || ch == '<' || ch == '>') {
            tokens.push_back({"OPERATOR", std::string(1, ch)});
            pos++;
            continue;
        }

        // Punctuation
        if (ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
            ch == ';' || ch == ':' || ch == ',') {
            tokens.push_back({"PUNCTUATION", std::string(1, ch)});
            pos++;
            continue;
        }

        // Anything else: unknown single character
        tokens.push_back({"UNKNOWN", std::string(1, ch)});
        pos++;
    }

    return tokens;
}
