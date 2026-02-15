#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// Base class for all AST nodes
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual std::string toString(int indent = 0) const = 0;
};

// Helper to create indentation
inline std::string indentStr(int level) {
    return std::string(level * 2, ' ');
}

// A number like 42
struct NumberLiteral : ASTNode {
    std::string value;

    NumberLiteral(const std::string& val) : value(val) {}

    std::string toString(int indent = 0) const override {
        return indentStr(indent) + "NumberLiteral(" + value + ")";
    }
};

// An identifier like x, foo, counter
struct Identifier : ASTNode {
    std::string name;

    Identifier(const std::string& name) : name(name) {}

    std::string toString(int indent = 0) const override {
        return indentStr(indent) + "Identifier(" + name + ")";
    }
};

// A string literal like "hello"
struct StringLiteral : ASTNode {
    std::string value;

    StringLiteral(const std::string& val) : value(val) {}

    std::string toString(int indent = 0) const override {
        return indentStr(indent) + "StringLiteral(\"" + value + "\")";
    }
};

// A binary expression like 1 + 2
struct BinaryExpr : ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryExpr(const std::string& op,
               std::unique_ptr<ASTNode> left,
               std::unique_ptr<ASTNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    std::string toString(int indent = 0) const override {
        std::string result = indentStr(indent) + "BinaryExpr(" + op + ")\n";
        result += left->toString(indent + 1) + "\n";
        result += right->toString(indent + 1);
        return result;
    }
};

// A let declaration like: let x = 5;
struct LetDecl : ASTNode {
    std::string name;
    bool isMut;
    std::unique_ptr<ASTNode> value;

    LetDecl(const std::string& name, bool isMut, std::unique_ptr<ASTNode> value)
        : name(name), isMut(isMut), value(std::move(value)) {}

    std::string toString(int indent = 0) const override {
        std::string mutStr = isMut ? "mut " : "";
        std::string result = indentStr(indent) + "LetDecl(" + mutStr + name + ")\n";
        result += value->toString(indent + 1);
        return result;
    }
};

// A function declaration like: fn main() { ... }
struct FunctionDecl : ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> body;

    FunctionDecl(const std::string& name, std::vector<std::unique_ptr<ASTNode>> body)
        : name(name), body(std::move(body)) {}

    std::string toString(int indent = 0) const override {
        std::string result = indentStr(indent) + "FunctionDecl(" + name + ")\n";
        for (const auto& stmt : body) {
            result += stmt->toString(indent + 1) + "\n";
        }
        if (!body.empty()) {
            result.pop_back(); // remove trailing newline
        }
        return result;
    }
};

// An if/else statement like: if x > 5 { ... } else { ... }
struct IfStatement : ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> thenBody;
    std::vector<std::unique_ptr<ASTNode>> elseBody; // empty if no else

    IfStatement(std::unique_ptr<ASTNode> condition,
                std::vector<std::unique_ptr<ASTNode>> thenBody,
                std::vector<std::unique_ptr<ASTNode>> elseBody)
        : condition(std::move(condition)),
          thenBody(std::move(thenBody)),
          elseBody(std::move(elseBody)) {}

    std::string toString(int indent = 0) const override {
        std::string result = indentStr(indent) + "IfStatement\n";
        result += indentStr(indent + 1) + "Condition:\n";
        result += condition->toString(indent + 2) + "\n";
        result += indentStr(indent + 1) + "Then:\n";
        for (const auto& stmt : thenBody) {
            result += stmt->toString(indent + 2) + "\n";
        }
        if (!elseBody.empty()) {
            result += indentStr(indent + 1) + "Else:\n";
            for (const auto& stmt : elseBody) {
                result += stmt->toString(indent + 2) + "\n";
            }
        }
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        return result;
    }
};

// A while loop like: while x > 0 { ... }
struct WhileStatement : ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;

    WhileStatement(std::unique_ptr<ASTNode> condition,
                   std::vector<std::unique_ptr<ASTNode>> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    std::string toString(int indent = 0) const override {
        std::string result = indentStr(indent) + "WhileStatement\n";
        result += indentStr(indent + 1) + "Condition:\n";
        result += condition->toString(indent + 2) + "\n";
        result += indentStr(indent + 1) + "Body:\n";
        for (const auto& stmt : body) {
            result += stmt->toString(indent + 2) + "\n";
        }
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        return result;
    }
};

// A return statement like: return x + 1;
struct ReturnStatement : ASTNode {
    std::unique_ptr<ASTNode> value;

    ReturnStatement(std::unique_ptr<ASTNode> value)
        : value(std::move(value)) {}

    std::string toString(int indent = 0) const override {
        std::string result = indentStr(indent) + "ReturnStatement\n";
        result += value->toString(indent + 1);
        return result;
    }
};

#endif
