#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>

class ASTNode
{
public:
    virtual ~ASTNode() = default;

    virtual void print() const
    {
        std::cout << "nodeName" << ":\n";
    }

protected:
    std::string nodeName;
};

class ProgramNode : public ASTNode
{
public:
    ProgramNode()
    {
        nodeName = "ProgramNode";
    }

    std::vector<std::unique_ptr<ASTNode>> declarations;

    void print() const override
    {
        ASTNode::print();

        for (const auto &decl : declarations)
        {
            decl->print();
        }
    }
};

// EXPRESIONS

class GlobalDeclarationNode : public ASTNode
{
    GlobalDeclarationNode()
    {
        nodeName = "GlobalDeclarationNode";
    }
    std::vector<std::unique_ptr<ASTNode>> globalDecls;
    void addDeclaration(std::unique_ptr<ASTNode> decl)
    {
        globalDecls.push_back(std::move(decl));
    }
};

class BinaryOpNode : public ASTNode
{
public:
    BinaryOpNode()
    {
        nodeName = "BinaryOpNode";
    }
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    std::string op;

    BinaryOpNode(std::unique_ptr<ASTNode> left, const std::string &op, std::unique_ptr<ASTNode> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
};

class AssignmentNode : public ASTNode
{
public:
    AssignmentNode()
    {
        nodeName = "AssignmentNode";
    }
    std::string id;
    std::unique_ptr<ASTNode> expr;

    AssignmentNode(const std::string &id, std::unique_ptr<ASTNode> expr)
        : id(id), expr(std::move(expr)) {}
};

// STATEMENTS

// add statements node here: jump, labaled, iterative, etc

// TERMINAL nodes
class ConstantIntegerNode : public ASTNode
{
public:
    int value;
    explicit ConstantIntegerNode(int value) : value(value)
    {
        nodeName = "ConstIntegerNode";
    }
    void print() const override
    {
        ASTNode::print();
        std::cout << value << std::endl;
    }
};

class ConstantFloatNode : public ASTNode
{
public:
    float value;
    explicit ConstantFloatNode(float value) : value(value)
    {
        nodeName = "ConstFloatNode";
    }
    void print() const override
    {
        ASTNode::print();
        std::cout << value << std::endl;
    }
};

class StringLiteralNode : public ASTNode
{
public:
    std::string value;
    explicit StringLiteralNode(std::string value) : value(value)
    {
        nodeName = "StringLiteralNode";
    }
    void print() const override
    {
        ASTNode::print();
        std::cout << value << std::endl;
    }
};