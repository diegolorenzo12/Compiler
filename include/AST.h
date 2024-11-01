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

/*
ABSTRACT CLASES
*/

class ListNode : public ASTNode
{
public:
    std::vector<std::unique_ptr<ASTNode>> nodes;

    // Method to add a declaration if it is not null
    void push_back_node(std::unique_ptr<ASTNode> node)
    {
        if (node)
        {
            nodes.push_back(std::move(node)); // Only add non-null declarations
        }
    }

    void push_back_vec(std::unique_ptr<ListNode> nodeList)
    {
        if (nodeList)
        {
            for (auto &node : nodeList->nodes)
            {
                nodes.push_back(std::move(node));
            }
        }
    }
    // Override the print method to print all declarations
    void print() const override
    {
        ASTNode::print(); // Call print on base class
        for (const auto &node : nodes)
        {
            node->print(); // Print each declaration
        }
    }
};

class BinaryASTNode : public ASTNode
{
public:
    BinaryASTNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : leftOperand(std::move(left)), rightOperand(std::move(right)) {}

protected:
    std::unique_ptr<ASTNode> leftOperand;
    std::unique_ptr<ASTNode> rightOperand;

    virtual void printOperator() const = 0;
};

class AndASTNode : public BinaryASTNode
{
public:
    using BinaryASTNode::BinaryASTNode; // Inherit constructors

    // Override the print method
    void printOperator() const override
    {
        std::cout << " AND "; // Print the AND operator
    }
};

class OrASTNode : public BinaryASTNode
{
public:
    using BinaryASTNode::BinaryASTNode; // Inherit constructors

    // Override the print method
    void printOperator() const override
    {
        std::cout << " OR "; // Print the OR operator
    }
};

// legacy system failure

/*
CONCRETE CLASSES FOR MY AST
*/

class ProgramNode : public ListNode
{
public:
    ProgramNode()
    {
        nodeName = "ProgramNode";
    }

    std::vector<std::unique_ptr<ASTNode>> declarations;
};

// DECLARATIONS

class GlobalDeclarationsNode : public ASTNode
{
public:
    GlobalDeclarationsNode()
    {
        nodeName = "GlobalDeclarationsNode";
    }
};

class DeclarationNode : public ASTNode
{
public:
    DeclarationNode()
    {
        nodeName = "DeclarationNode";
    }
    std::unique_ptr<ASTNode> declaration;

    void setDeclaration(std::unique_ptr<ASTNode> decl)
    {
        if (decl)
        {
            declaration = std::move(decl);
        }
    }

    virtual ~DeclarationNode() = default;
};

// EXPRESIONS
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