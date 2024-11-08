#include "ASTVisitor.h"
#include <iostream>

/*
PRINT ABSTRACT SYNTAX TREE
*/

class PrintVisitor : public ASTVisitor
{
public:
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

    void visit(Program &node) override
    {
        std::cout << "Program {\n";
        for (const auto &decl : node.getTopLevelDeclarations())
        {
            acceptIfNotNull(decl);
            std::cout << "\n";
        }
        std::cout << "}\n";
    }

    void visit(BinaryExpr &node) override
    {
        std::cout << "{";
        acceptIfNotNull(node.getLHS());
        std::cout << " " << node.getOp() << " ";
        acceptIfNotNull(node.getRHS());
        std::cout << "}";
    }

    void visit(LiteralExpr &node) override
    {
        std::cout << node.getValue();
    }

    void visit(VariableExpr &node) override
    {
        std::cout << node.getName();
    }

    void visit(FunctionDecl &node) override
    {
        std::cout << "Function {\n";
        acceptIfNotNull(node.getDeclarationSpecifiers());
        acceptIfNotNull(node.getDeclarator());
        acceptIfNotNull(node.getBody());
        std::cout << "}\n";
    }

    void visit(Declaration &node) override
    {
        std::cout << "Declaration: {\n";
        acceptIfNotNull(node.getDeclarationSpecifiers());
        acceptIfNotNull(node.getDeclaratorList());
        std::cout << "}\n";
    }

    void visit(PrimitiveType &node) override
    {
        std::cout << "Type: " << node.getBaseType() << " ";
    }

    void visit(Specifier &node) override
    {
        std::cout << "Specifier:" << node.getSpecifier() << ", SpecifierType:" << node.getSpecifierType();
    }

    void visit(DeclarationSpecifiers &node) override
    {
        std::cout << "DeclarationSpecifiers: {";

        acceptIfNotNull(node.getType());
        acceptIfNotNull(node.getSpecifiers());
        std::cout << "}\n";
    }

    void visit(Declarator &node) override
    {
        std::cout << "Declarator:{ \n Identifier: " << node.getIdentifier() << ", \n";

        if (node.isPointer())
        {
            std::cout << "isPointer: true, \n";
        }
        acceptIfNotNull(node.getDeclarator());
        for (const auto &decl : node.getDirectDeclarators())
        {
            acceptIfNotNull(decl);
        }
        acceptIfNotNull(node.getInitializer());
    }

    void visit(DeclaratorList &node) override
    {
        std::cout << "DeclaratorList {\n";
        for (const auto &decl : node.getDeclarators())
        {
            acceptIfNotNull(decl);
        }
        std::cout << "}\n";
    }

    void visit(ExpressionInitializer &node) override
    {
        std::cout << "Initializer ";
        acceptIfNotNull(node.getExpr());
    }

    void visit(BraceInitializer &node) override
    {
        std::cout << "BraceInitializer";
        acceptIfNotNull(node.getInitializerList());
    }

    void visit(InitializerList &node) override
    {
        std::cout << "BraceInitializer";
        for (const auto &initializer : node.getInitializerList())
        {
            acceptIfNotNull(initializer);
            std::cout << "\n";
        }
    }

    void visit(BlockStmt &node) override
    {
        std::cout << "BlockStmt {\n";
        for (const auto &item : node.getItems())
        {
            acceptIfNotNull(item);
            std::cout << "\n";
        }
        std::cout << "}\n";
    }

    void visit(DeclarationWrapper &node) override
    {
        std::cout << "DeclarationWrapper ";
        acceptIfNotNull(node.getDeclaration());
    }

    void visit(StatementWrapper &node) override
    {
        std::cout << "StatementWrapper ";
        acceptIfNotNull(node.getStatement());
    }

    void visit(CaseStatement &node) override
    {
        std::cout << "CaseStatement ";
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getStatement());
    }

    void visit(DefaultStatement &node) override
    {
        std::cout << "DefaultStatement ";
        acceptIfNotNull(node.getStatement());
    }

    void visit(ExpressionStatement &node) override
    {
        std::cout << "ExpressionStatement ";
        acceptIfNotNull(node.getExpr());
    }

    void visit(SwitchStatement &node) override
    {
        std::cout << "SwitchStatement ";
        acceptIfNotNull(node.getCondition());
        acceptIfNotNull(node.getBody());
    }

    void visit(IfStatement &node) override
    {
        std::cout << "IfStatement ";
        acceptIfNotNull(node.getCondition());
        acceptIfNotNull(node.getThen());
    }

    void visit(WhileStatement &node) override
    {
        std::cout << "WhileStatement ";
        acceptIfNotNull(node.getCondition());
        acceptIfNotNull(node.getBody());
    }

    void visit(DoWhileStatement &node) override
    {
        std::cout << "DoWhileStatement ";

        acceptIfNotNull(node.getCondition());
        acceptIfNotNull(node.getBody());
    }

    void visit(ForStatement &node) override
    {
        std::cout << "ForStatement ";
        acceptIfNotNull(node.getInit());
        acceptIfNotNull(node.getStatement());
    }

    void visit(ForWitDeclaration &node) override
    {
        std::cout << "ForWitDeclaration ";
        acceptIfNotNull(node.getDeclaration());
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getOptionalExpr());
    }

    void visit(ForWithExpression &node) override
    {
        std::cout << "ForWithExpression ";
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getExpr2());
        acceptIfNotNull(node.getOptionalExpr());
    }

    void visit(ReturnStatement &node) override
    {
        std::cout << "ReturnStatement ";
        acceptIfNotNull(node.getExpr());
    }

    void visit(ContinueStatement &node) override
    {
        std::cout << "ContinueStatement ";
    }

    void visit(BreakStatement &node) override
    {
        std::cout << "BreakStatement ";
    }

    void visit(ConditionalExpression &node) override
    {
        std::cout << "ConditionalExpression ";
        acceptIfNotNull(node.getLogicalOrExpression());
        acceptIfNotNull(node.getExpression());
        acceptIfNotNull(node.getConditionalExpression());
    }

    void visit(ParenthesizedExpression &node) override
    {
        std::cout << "ParenthesizedExpression ";
        acceptIfNotNull(node.getExpr());
    }

    void visit(ConstantExpression &node) override
    {
        std::cout << "ConstantExpression: { value: " << node.getConstant() << ", type: " << node.getConstantTypeString() << " }";
    }

    void visit(IdentifierExpression &node) override
    {
        std::cout << "IdentifierExpression(" << node.getIdentifier() << ")";
    }

    void visit(ArrayPostFixExpression &node) override
    {
        std::cout << "ArrayPostFixExpression ";
        acceptIfNotNull(node.getArraySize());
    }

    void visit(AssignmentExpressionList &node) override
    {
        std::cout << "AssignmentExpressionList ";
        for (const auto &arg : node.getAssignmentExpression())
        {
            acceptIfNotNull(arg);
        }
    }

    void visit(AssignmentOperator &node) override
    {
        std::cout << "AssignmentOperator: " << node.getOp();
    }

    void visit(AssignmentExpression &node) override
    {
        std::cout << "AssignmentExpression ";
        acceptIfNotNull(node.getConditionalExpression());
        acceptIfNotNull(node.getAssignmentOperator());
        acceptIfNotNull(node.getAssignmentExpression());
    }

    void visit(ArgumentsPostFixExpression &node) override
    {
        std::cout << "ArgumentsPostFixExpression: ";
        acceptIfNotNull(node.getArgumentList());
    }

    void visit(DotOperatorPostfixExpression &node) override
    {
        std::cout << "DotOperatorPostfixExpression " << node.getIdentifier() << " ";
    }

    void visit(ArrowOperatorPostfixExpression &node) override
    {
        std::cout << "ArrowOperatorPostfixExpression " << node.getIdentifier() << " ";
    }

    void visit(IncrementDecrementPostfixExpression &node) override
    {
        std::cout << "IncrementDecrementPostfixExpression " << node.getOp() << " ";
    }

    void visit(PostfixExpressions &node) override
    {
        std::cout << "PostfixExpressions ";
        acceptIfNotNull(node.getPrimaryExpression());
        for (const auto &expr : node.getPostfixExpressions())
        {
            acceptIfNotNull(expr);
        }
    }

    void visit(UnaryIncrementDecrementOperator &node) override
    {
        std::cout << "UnaryIncrementDecrementOperator " << node.getOp() << " {";
        acceptIfNotNull(node.getUnaryExpr());
        std::cout << "}";
    }

    void visit(IdentifierList &node) override
    {
        std::cout << "IdentifierList {\n";
        for (const auto &id : node.getIdentifiers())
        {
            std::cout << id << " ";
        }
        std::cout << "}\n";
    }

    void visit(ArrayDirectDeclarator &node) override
    {
        std::cout << "ArrayDirectDeclarator {\n";
        if (node.getIsStatic())
        {
            std::cout << "IsStatic:True, ";
        }
        acceptIfNotNull(node.getTypeQualifier());
        acceptIfNotNull(node.getArrSize());
        std::cout << "}\n";
    }

    void visit(ParameterDeclaration &node) override
    {
        std::cout << "ParameterDeclaration {\n";
        acceptIfNotNull(node.getDeclarationSpecifiers());
        acceptIfNotNull(node.getDeclarator());
        std::cout << "}\n";
    }

    void visit(ParameterDeclarationList &node) override
    {
        std::cout << "ParameterDeclarationList {\n";
        for (const auto &param : node.getParameters())
        {
            acceptIfNotNull(param);
        }
        std::cout << "}\n";
    }

    void visit(AnonimousStruct &node) override
    {
        std::cout << "AnonimousStruct {\n";
        acceptIfNotNull(node.getStructDeclarationList());
        std::cout << "}\n";
    }

    void visit(StructDeclaration &node) override
    {
        std::cout << "StructDeclaration {\n";
        std::cout << "Identifier: " << node.getIdentifier() << "\n";
        acceptIfNotNull(node.getStructDeclarationList());
        std::cout << "}\n";
    }

    void visit(StructMemberDeclarationList &node) override
    {
        std::cout << "StructMemberDeclarationList {\n";
        for (const auto &member : node.getStructMemberDeclarations())
        {
            acceptIfNotNull(member);
        }
        std::cout << "}\n";
    }

    void visit(StructMemberDeclaration &node) override
    {
        std::cout << "StructMemberDeclaration {\n";
        acceptIfNotNull(node.getTypeSpecifier());
        acceptIfNotNull(node.getDeclaratorList());
        std::cout << "}\n";
    }
};