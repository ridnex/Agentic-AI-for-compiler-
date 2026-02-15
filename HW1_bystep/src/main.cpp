#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: rustlex <file.rs>" << std::endl;
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

    // Tokenize
    Lexer lexer;
    std::vector<Token> tokens = lexer.tokenize(source);

    // Print each token
    for (const Token& t : tokens) {
        std::cout << t.type << ": " << t.value << std::endl;
    }

    return 0;
}
