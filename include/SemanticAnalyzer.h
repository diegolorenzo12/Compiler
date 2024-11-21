#include "ASTVisitor.h"
#include <iostream>
#include "SymbolTable.h"

/*
Semantic Analyzer ABSTRACT SYNTAX TREE visitor
*/

class SemanticAnalyzer : public ASTVisitor
{
    std::shared_ptr<SymbolTable> symbolTable;

public:
    explicit SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : symbolTable(std::move(symbolTable)) {}

    void acceptIfNotNull(ASTNode *node)
    {
        if (node != nullptr)
        {
            node->accept(*this);
        }
    }

    void acceptIfNotNull(const std::unique_ptr<ASTNode> &node)
    {
        acceptIfNotNull(node.get());
    }

    template <typename NodeType>
    void acceptIfNotNull(const std::unique_ptr<NodeType> &node)
    {
        acceptIfNotNull(node.get()); // Calls the raw pointer version if valid
    }

    void visit(Program &node) = 0;
    void visit(BinaryExpr &node) = 0;
    void visit(LiteralExpr &node) = 0;
    void visit(VariableExpr &node) = 0;
    void visit(FunctionDecl &node) = 0;
    void visit(Declaration &node) = 0;
    void visit(PrimitiveType &node) = 0;
    void visit(Specifier &node) = 0;
    void visit(DeclarationSpecifiers &node) = 0;
    void visit(Declarator &node) = 0;
    void visit(DeclaratorList &node) = 0;
    void visit(ExpressionInitializer &node) = 0;
    void visit(BraceInitializer &node) = 0;
    void visit(InitializerList &node) = 0;
    void visit(BlockStmt &node) = 0;
    void visit(DeclarationWrapper &node) = 0;
    void visit(StatementWrapper &node) = 0;
    void visit(CaseStatement &node) = 0;
    void visit(DefaultStatement &node) = 0;
    void visit(ExpressionStatement &node) = 0;
    void visit(SwitchStatement &node) = 0;
    void visit(IfStatement &node) = 0;
    void visit(ForStatement &node) = 0;
    void visit(WhileStatement &node) = 0;
    void visit(DoWhileStatement &node) = 0;
    void visit(ForWitDeclaration &node) = 0;
    void visit(ForWithExpression &node) = 0;
    void visit(ReturnStatement &node) = 0;
    void visit(ContinueStatement &node) = 0;
    void visit(BreakStatement &node) = 0;
    void visit(ConditionalExpression &node) = 0;
    void visit(ParenthesizedExpression &node) = 0;
    void visit(ConstantExpression &node) = 0;
    void visit(IdentifierExpression &node) = 0;
    void visit(ArrayPostFixExpression &node) = 0;
    void visit(AssignmentExpressionList &node) = 0;
    void visit(AssignmentOperator &node) = 0;
    void visit(AssignmentExpression &node) = 0;
    void visit(ArgumentsPostFixExpression &node) = 0;
    void visit(DotOperatorPostfixExpression &node) = 0;
    void visit(ArrowOperatorPostfixExpression &node) = 0;
    void visit(IncrementDecrementPostfixExpression &node) = 0;
    void visit(PostfixExpressions &node) = 0;
    void visit(UnaryIncrementDecrementOperator &node) = 0;
    void visit(IdentifierList &node) = 0;
    void visit(ArrayDirectDeclarator &node) = 0;
    void visit(ParameterDeclaration &node) = 0;
    void visit(ParameterDeclarationList &node) = 0;
    void visit(AnonimousStruct &node) = 0;
    void visit(StructDeclaration &node) = 0;
    void visit(StructMemberDeclarationList &node) = 0;
    void visit(StructMemberDeclaration &node) = 0;
};