#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: rustparser <file.rs>" << std::endl;
        return 1;
    }

    // Read the file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Step 1: Tokenize
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(source);

    std::cout << "=== Tokens ===" << std::endl;
    for (const Token& t : tokens) {
        std::cout << "  " << t.type << ": " << t.value << std::endl;
    }
    std::cout << std::endl;

    // Step 2: Parse
    Parser parser;
    try {
        auto ast = parser.parse(tokens);

        std::cout << "=== AST ===" << std::endl;
        for (const auto& node : ast) {
            std::cout << node->toString() << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Parse error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
