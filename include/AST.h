#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>

class ASTNode {
public:
    virtual void print(int indent = 0) const = 0;
    virtual ~ASTNode() = default;
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> children;

    void addChild(std::unique_ptr<ASTNode> child) {
        children.push_back(std::move(child));
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ProgramNode" << std::endl;
        for (const auto& child : children) {
            child->print(indent + 2);
        }
    }
};

class DeclarationNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> declaration;

    void setDeclaration(std::unique_ptr<ASTNode> decl) {
        declaration = std::move(decl);
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "DeclarationNode" << std::endl;
        if (declaration) {
            declaration->print(indent + 2);
        }
    }
};

class ExpressionNode : public ASTNode {
public:
    std::string value;
    ExpressionNode(const std::string& value) : value(value) {}

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "ExpressionNode: " << value << std::endl;
    }
};

class BlockNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    void addStatement(std::unique_ptr<ASTNode> stmt) {
        statements.push_back(std::move(stmt));
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "BlockNode" << std::endl;
        for (const auto& stmt : statements) {
            if (stmt) {
                stmt->print(indent + 2);
            }
        }
    }
};

class FunctionDefinitionNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> declarationSpecifiers;
    std::unique_ptr<ASTNode> declarator;
    std::unique_ptr<BlockNode> body;

    FunctionDefinitionNode() = default;

    void setDeclarationSpecifiers(std::unique_ptr<ASTNode> specifiers) {
        declarationSpecifiers = std::move(specifiers);
    }

    void setDeclarator(std::unique_ptr<ASTNode> decl) {
        declarator = std::move(decl);
    }

    void setBody(std::unique_ptr<BlockNode> block) {
        body = std::move(block);
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "FunctionDefinitionNode" << std::endl;
        if (declarationSpecifiers) {
            declarationSpecifiers->print(indent + 2);
        }
        if (declarator) {
            declarator->print(indent + 2);
        }
        if (body) {
            body->print(indent + 2);
        }
    }
};
