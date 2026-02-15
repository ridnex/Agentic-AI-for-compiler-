#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "lexer.h"
#include "ast.h"

class Parser {
public:
    std::vector<std::unique_ptr<ASTNode>> parse(const std::vector<Token>& tokens);

private:
    const std::vector<Token>* tokens_ = nullptr;
    int pos_ = 0;

    const Token& current() const;
    const Token& peek() const;
    bool atEnd() const;
    const Token& advance();
    void expect(const std::string& type, const std::string& value);

    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseLetDecl();
    std::unique_ptr<ASTNode> parseFunctionDecl();
    std::vector<std::unique_ptr<ASTNode>> parseBlock();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseWhileStatement();
    std::unique_ptr<ASTNode> parseReturnStatement();
};

#endif
