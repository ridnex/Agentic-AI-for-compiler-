#include "parser.h"
#include <iostream>

// --- Utility methods ---

const Token& Parser::current() const {
    return (*tokens_)[pos_];
}

const Token& Parser::peek() const {
    return (*tokens_)[pos_];
}

bool Parser::atEnd() const {
    return pos_ >= (int)tokens_->size();
}

const Token& Parser::advance() {
    const Token& tok = (*tokens_)[pos_];
    pos_++;
    return tok;
}

void Parser::expect(const std::string& type, const std::string& value) {
    if (atEnd()) {
        throw std::runtime_error("Expected " + type + " '" + value + "' but reached end of input");
    }
    const Token& tok = current();
    if (tok.type != type || tok.value != value) {
        throw std::runtime_error("Expected " + type + " '" + value + "' but got " + tok.type + " '" + tok.value + "'");
    }
    advance();
}

// --- Parsing ---

std::vector<std::unique_ptr<ASTNode>> Parser::parse(const std::vector<Token>& tokens) {
    tokens_ = &tokens;
    pos_ = 0;

    std::vector<std::unique_ptr<ASTNode>> program;
    while (!atEnd()) {
        program.push_back(parseStatement());
    }
    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (!atEnd() && current().type == "KEYWORD") {
        if (current().value == "let") return parseLetDecl();
        if (current().value == "fn") return parseFunctionDecl();
        if (current().value == "if") return parseIfStatement();
        if (current().value == "while") return parseWhileStatement();
        if (current().value == "return") return parseReturnStatement();
    }
    return parseExpression();
}

// Parse: { stmt1; stmt2; ... }
std::vector<std::unique_ptr<ASTNode>> Parser::parseBlock() {
    expect("PUNCTUATION", "{");
    std::vector<std::unique_ptr<ASTNode>> body;
    while (!atEnd() && !(current().type == "PUNCTUATION" && current().value == "}")) {
        body.push_back(parseStatement());
    }
    expect("PUNCTUATION", "}");
    return body;
}

// Parse: fn name() { body }
std::unique_ptr<ASTNode> Parser::parseFunctionDecl() {
    expect("KEYWORD", "fn");

    if (atEnd() || current().type != "IDENTIFIER") {
        throw std::runtime_error("Expected function name after 'fn'");
    }
    std::string name = current().value;
    advance();

    expect("PUNCTUATION", "(");
    expect("PUNCTUATION", ")");

    auto body = parseBlock();

    return std::make_unique<FunctionDecl>(name, std::move(body));
}

// Parse: let [mut] name = expr ;
std::unique_ptr<ASTNode> Parser::parseLetDecl() {
    expect("KEYWORD", "let");

    bool isMut = false;
    if (!atEnd() && current().type == "KEYWORD" && current().value == "mut") {
        isMut = true;
        advance();
    }

    if (atEnd() || current().type != "IDENTIFIER") {
        throw std::runtime_error("Expected variable name after 'let'");
    }
    std::string name = current().value;
    advance();

    expect("OPERATOR", "=");

    auto value = parseExpression();

    expect("PUNCTUATION", ";");

    return std::make_unique<LetDecl>(name, isMut, std::move(value));
}

// Parse a primary value: number, string, or identifier
std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (atEnd()) {
        throw std::runtime_error("Unexpected end of input while parsing expression");
    }

    const Token& tok = current();

    if (tok.type == "NUMBER") {
        advance();
        return std::make_unique<NumberLiteral>(tok.value);
    }

    if (tok.type == "STRING") {
        advance();
        return std::make_unique<StringLiteral>(tok.value);
    }

    if (tok.type == "IDENTIFIER") {
        advance();
        return std::make_unique<Identifier>(tok.value);
    }

    throw std::runtime_error("Unexpected token: " + tok.type + " '" + tok.value + "'");
}

// Parse: if expr { body } [else { body }]
std::unique_ptr<ASTNode> Parser::parseIfStatement() {
    expect("KEYWORD", "if");

    auto condition = parseExpression();
    auto thenBody = parseBlock();

    std::vector<std::unique_ptr<ASTNode>> elseBody;
    if (!atEnd() && current().type == "KEYWORD" && current().value == "else") {
        advance();
        elseBody = parseBlock();
    }

    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBody), std::move(elseBody));
}

// Parse: return expr ;
std::unique_ptr<ASTNode> Parser::parseReturnStatement() {
    expect("KEYWORD", "return");

    auto value = parseExpression();

    expect("PUNCTUATION", ";");

    return std::make_unique<ReturnStatement>(std::move(value));
}

// Parse: while expr { body }
std::unique_ptr<ASTNode> Parser::parseWhileStatement() {
    expect("KEYWORD", "while");

    auto condition = parseExpression();
    auto body = parseBlock();

    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

// Parse expression: primary, optionally followed by operator + primary
std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto left = parsePrimary();

    // If next token is an operator, parse binary expression
    while (!atEnd() && current().type == "OPERATOR") {
        std::string op = current().value;
        advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}
