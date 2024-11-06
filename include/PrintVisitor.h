#include "ASTVisitor.h"
#include <iostream>

/*
PRINT ABSTRACT SYNTAX TREE
*/

class PrintVisitor : public ASTVisitor
{
public:
    void visit(Program &node) override
    {
        std::cout << "Program {\n";
        for (const auto &decl : node.getTopLevelDeclarations())
        {
            if (decl != nullptr)
            {
                decl->accept(*this);
                std::cout << "\n";
            }
        }
        std::cout << "}\n";
    }

    void visit(BinaryExpr &node) override
    {
        std::cout << "(";
        node.getLHS()->accept(*this);
        std::cout << " " << node.getOp() << " ";
        node.getRHS()->accept(*this);
        std::cout << ")";
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
        std::cout << "Function " << " {\n";
        if (node.getDeclarationSpecifiers() != nullptr)
        {
            node.getDeclarationSpecifiers()->accept(*this);
        }
        if (node.getDeclarator() != nullptr)
        {
            node.getDeclarator()->accept(*this);
        }
        if (node.getBody() != nullptr)
        {
            node.getBody()->accept(*this);
        }

        std::cout << "}\n";
    }

    void visit(Declaration &node) override
    {
        std::cout << "Declaration: {\n";
        if (node.getDeclarationSpecifiers() != nullptr)
        {
            node.getDeclarationSpecifiers()->accept(*this);
        }
        if (node.getDeclaratorList() != nullptr)
        {
            std::cout << ", \n";
            node.getDeclaratorList()->accept(*this);
        }
        std::cout << "}\n";
    }

    void visit(Type &node) override
    {
        std::cout << "Type (" << node.getBaseType() << ")";
    }

    void visit(Specifier &node) override
    {
        std::cout << "Specifier " << node.getSpecifier() << " " << node.getSpecifierType();
    }

    void visit(DeclarationSpecifiers &node) override
    {
        std::cout << "DeclarationSpecifiers: ";

        if (node.getType() != nullptr)
        {
            node.getType()->accept(*this);
        }
        if (node.getSpecifiers() != nullptr)
        {
            std::cout << "Type: ";
            node.getSpecifiers()->accept(*this);
            std::cout << "\n";
        }
    }

    void visit(Declarator &node) override
    {
        std::cout << "Declarator, Identifier(" << node.getIdentifier() << ")";

        if (node.isPointer())
        {
            std::cout << "Is Pointer \n";
        }
        if (node.isArray())
        {
            std::cout << "Is Array \n";
        }
        if (node.isFunction())
        {
            std::cout << " Is Function \n";
        }
        if (node.hasInitializer())
        {
            node.getInitializer()->accept(*this);
        }
    }

    void visit(DeclatatorList &node) override
    {
        std::cout << "DeclaratorList {\n";
        for (const auto &decl : node.getDeclarators())
        {

            if (decl != nullptr)
            {
                decl->accept(*this);
                std::cout << "\n";
            }
        }
        std::cout << "}\n";
    }

    void visit(ExpressionInitializer &node) override
    {
        std::cout << "Initializer ";
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
    }

    void visit(BraceInitializer &node) override
    {
        std::cout << "BraceInitializer";
        if (node.getInitializerList() != nullptr)
        {
            node.getInitializerList()->accept(*this);
        }
    }

    void visit(InitializerList &node) override
    {
        std::cout << "BraceInitializer";
        for (const auto &initializer : node.getInitializerList())
        {
            if (initializer != nullptr)
            {
                initializer->accept(*this);
                std::cout << "\n";
            }
        }
    }

    void visit(BlockStmt &node) override
    {
        std::cout << "BlockStmt {\n";
        for (const auto &item : node.getItems())
        {
            item->accept(*this);
            std::cout << "\n";
        }
        std::cout << "}\n";
    }

    void visit(DeclarationWrapper &node) override
    {
        std::cout << "DeclarationWrapper ";
        if (node.getDeclaration() != nullptr)
        {
            node.getDeclaration()->accept(*this);
        }
    }

    void visit(StatementWrapper &node) override
    {
        std::cout << "StatementWrapper ";
        if (node.getStatement() != nullptr)
        {
            node.getStatement()->accept(*this);
        }
    }

    void visit(CaseStatement &node) override
    {
        std::cout << "CaseStatement ";
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
        if (node.getStatement() != nullptr)
        {
            node.getStatement()->accept(*this);
        }
    }

    void visit(DefaultStatement &node) override
    {
        std::cout << "DefaultStatement ";
        if (node.getStatement() != nullptr)
        {
            node.getStatement()->accept(*this);
        }
    }

    void visit(ExpressionStatement &node) override
    {
        std::cout << "ExpressionStatement ";
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
    }

    void visit(SwitchStatement &node) override
    {
        std::cout << "SwitchStatement ";
        if (node.getCondition() != nullptr)
        {
            node.getCondition()->accept(*this);
        }
        if (node.getBody() != nullptr)
        {
            node.getBody()->accept(*this);
        }
    }

    void visit(IfStatement &node) override
    {
        std::cout << "IfStatement ";
        if (node.getCondition() != nullptr)
        {
            node.getCondition()->accept(*this);
        }
        if (node.getThen() != nullptr)
        {
            node.getThen()->accept(*this);
        }
    }

    void visit(WhileStatement &node) override
    {
        std::cout << "WhileStatement ";
        if (node.getCondition() != nullptr)
        {
            node.getCondition()->accept(*this);
        }
        if (node.getBody() != nullptr)
        {
            node.getBody()->accept(*this);
        }
    }

    void visit(DoWhileStatement &node) override
    {
        std::cout << "DoWhileStatement ";
        if (node.getCondition() != nullptr)
        {
            node.getCondition()->accept(*this);
        }
        if (node.getBody() != nullptr)
        {
            node.getBody()->accept(*this);
        }
    }

    void visit(ForStatement &node) override
    {
        std::cout << "ForStatement ";
        if (node.getInit() != nullptr)
        {
            node.getInit()->accept(*this);
        }
        if (node.getStatement() != nullptr)
        {
            node.getStatement()->accept(*this);
        }
    }

    void visit(ForWitDeclaration &node) override
    {
        std::cout << "ForWitDeclaration ";
        if (node.getDeclaration() != nullptr)
        {
            node.getDeclaration()->accept(*this);
        }
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
        if (node.getOptionalExpr() != nullptr)
        {
            node.getOptionalExpr()->accept(*this);
        }
    }

    void visit(ForWithExpression &node) override
    {
        std::cout << "ForWithExpression ";
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
        if (node.getExpr2() != nullptr)
        {
            node.getExpr2()->accept(*this);
        }
        if (node.getOptionalExpr() != nullptr)
        {
            node.getOptionalExpr()->accept(*this);
        }
    }

    void visit(ReturnStatement &node) override
    {
        std::cout << "ReturnStatement ";
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
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
        if (node.getLogicalOrExpression() != nullptr)
        {
            node.getLogicalOrExpression()->accept(*this);
        }
        if (node.getExpression() != nullptr)
        {
            node.getExpression()->accept(*this);
            if (node.getConditionalExpression() != nullptr)
            {
                node.getConditionalExpression()->accept(*this);
            }
        }
    }

    void visit(ParenthesizedExpression &node) override
    {
        std::cout << "ParenthesizedExpression ";
        if (node.getExpr() != nullptr)
        {
            node.getExpr()->accept(*this);
        }
    }

    void visit(ConstantExpression &node) override
    {
        std::cout << "ConstantExpression(" << node.getConstant() << "), with tyoe: " << node.getConstantTypeString();
    }

    void visit(IdentifierExpression &node) override
    {
        std::cout << "IdentifierExpression(" << node.getIdentifier() << ")";
    }

    void visit(ArrayPostFixExpression &node) override
    {
        std::cout << "ArrayPostFixExpression ";
        if (node.getArraySize() != nullptr)
        {
            node.getArraySize()->accept(*this);
        }
    }
};
