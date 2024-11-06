#pragma once
#include "AST.h"

class ASTVisitor
{
public:
    virtual ~ASTVisitor() = default;

    // Visitor methods for each AST node type
    virtual void visit(Program &node) = 0;
    virtual void visit(BinaryExpr &node) = 0;
    virtual void visit(LiteralExpr &node) = 0;
    virtual void visit(VariableExpr &node) = 0;
    virtual void visit(FunctionDecl &node) = 0;
    virtual void visit(Declaration &node) = 0;
    virtual void visit(Type &node) = 0;
    virtual void visit(Specifier &node) = 0;
    virtual void visit(DeclarationSpecifiers &node) = 0;
    virtual void visit(Declarator &node) = 0;
    virtual void visit(DeclatatorList &node) = 0;
    virtual void visit(ExpressionInitializer &node) = 0;
    virtual void visit(BraceInitializer &node) = 0;
    virtual void visit(InitializerList &node) = 0;
    virtual void visit(BlockStmt &node) = 0;
    virtual void visit(DeclarationWrapper &node) = 0;
    virtual void visit(StatementWrapper &node) = 0;
    virtual void visit(CaseStatement &node) = 0;
    virtual void visit(DefaultStatement &node) = 0;
    virtual void visit(ExpressionStatement &node) = 0;
    virtual void visit(SwitchStatement &node) = 0;
    virtual void visit(IfStatement &node) = 0;
    virtual void visit(ForStatement &node) = 0;
    virtual void visit(WhileStatement &node) = 0;
    virtual void visit(DoWhileStatement &node) = 0;
    virtual void visit(ForWitDeclaration &node) = 0;
    virtual void visit(ForWithExpression &node) = 0;
    virtual void visit(ReturnStatement &node) = 0;
    virtual void visit(ContinueStatement &node) = 0;
    virtual void visit(BreakStatement &node) = 0;
    virtual void visit(ConditionalExpression &node) = 0;
    virtual void visit(ParenthesizedExpression &node) = 0;
    virtual void visit(ConstantExpression &node) = 0;
    virtual void visit(IdentifierExpression &node) = 0;
    virtual void visit(ArrayPostFixExpression &node) = 0;
};
