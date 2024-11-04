#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Forward declarations for visitor pattern
class Program;
class BinaryExpr;
class LiteralExpr;
class VariableExpr;
class FunctionDecl;
class Declaration;
class BlockStmt;
class Type;
class Specifier;
class DeclarationSpecifiers;
class Declarator;
class DeclatatorList;
class Initializer;
class InitializerList;
class BraceInitializer;
class ExpressionInitializer;
class DeclarationWrapper;
class StatementWrapper;
class LabaledStatement;
class CaseStatement;
class DefaultStatement;
class ExpressionStatement;
class SwitchStatement;
class IfStatement;
class WhileStatement;
class DoWhileStatement;
class ForStatement;
class ForWitDeclaration;
class ForWithExpression;
class ReturnStatement;
class ContinueStatement;
class BreakStatement;

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
};

// ASTVisitor base class for the visitor pattern

// Abstract base class for all AST nodes
class ASTNode
{
public:
    virtual ~ASTNode() = default;

    // Accept method for the visitor
    virtual void accept(ASTVisitor &visitor) = 0;
};

// Program node as the root of the AST
class Program : public ASTNode
{
public:
    Program(std::vector<std::unique_ptr<ASTNode>> topLevelDeclarations)
        : topLevelDeclarations_(std::move(topLevelDeclarations)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<ASTNode>> &getTopLevelDeclarations() const
    {
        return topLevelDeclarations_;
    }

private:
    std::vector<std::unique_ptr<ASTNode>> topLevelDeclarations_;
};

// Expression node base class
class Expr : public ASTNode
{
};

// Statement node base class
class Stmt : public ASTNode
{
};

class BinaryExpr : public Expr
{
public:
    BinaryExpr(std::unique_ptr<Expr> lhs, std::string op, std::unique_ptr<Expr> rhs)
        : lhs_(std::move(lhs)), op_(op), rhs_(std::move(rhs)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    // Getters for the sub-expressions
    Expr *getLHS() const { return lhs_.get(); }
    Expr *getRHS() const { return rhs_.get(); }
    const std::string &getOp() const { return op_; }

private:
    std::unique_ptr<Expr> lhs_, rhs_;
    std::string op_;
};

// Example: Literal Expression Node (e.g., "42")
class LiteralExpr : public Expr
{
public:
    explicit LiteralExpr(int value) : value_(value) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    int getValue() const { return value_; }

private:
    int value_;
};

// Example: Variable Expression Node (e.g., "x")
class VariableExpr : public Expr
{
public:
    explicit VariableExpr(const std::string &name) : name_(name) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getName() const { return name_; }

private:
    std::string name_;
};

// Function Declaration Node
class FunctionDecl : public ASTNode
{
public:
    FunctionDecl(std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers, std::unique_ptr<Declarator> declarator, std::unique_ptr<BlockStmt> block)
        : declarationSpecifiers_(std::move(declarationSpecifiers)), declarator_(std::move(declarator)), block_(std::move(block)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
    BlockStmt *getBody() const { return block_.get(); }
    DeclarationSpecifiers *getDeclarationSpecifiers() const { return declarationSpecifiers_.get(); }
    Declarator *getDeclarator() const { return declarator_.get(); }

private:
    std::unique_ptr<BlockStmt> block_;
    std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers_;
    std::unique_ptr<Declarator> declarator_;
};

class Type : public ASTNode
{
public:
    Type(std::string baseType, bool isPointer = false)
        : baseType_(std::move(baseType)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getBaseType() const { return baseType_; }

private:
    std::string baseType_;
};

class Specifier : public ASTNode
{
public:
    explicit Specifier(std::string specifier, std::string specifierType) : specifier_(std::move(specifier)), specifierType_(std::move(specifierType_)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getSpecifier() const { return specifier_; }
    const std::string &getSpecifierType() const { return specifierType_; }

private:
    std::string specifier_;
    std::string specifierType_;
};

class DeclarationSpecifiers : public ASTNode
{
public:
    DeclarationSpecifiers(std::unique_ptr<Type> type, std::unique_ptr<Specifier> specifiers)
        : type_(std::move(type)), specifier_(std::move(specifiers)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::unique_ptr<Type> &getType() const { return type_; }
    const std::unique_ptr<Specifier> &getSpecifiers() const { return specifier_; }

private:
    std::unique_ptr<Type> type_;
    std::unique_ptr<Specifier> specifier_;
};

struct arraySize // In direct declarator
{
    std::unique_ptr<ASTNode> expr;
    bool isStatic;
    std::string typeQualifier;
};

class Declarator : public ASTNode
{
public:
    Declarator() {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const void setIdentifier(std::string identifier) { identifier_ = identifier; }
    const void setPointer(bool isPointer) { isPointer_ = isPointer; }
    const bool isPointer() const { return isPointer_; }
    const std::string &getIdentifier() const { return identifier_; }
    const std::unique_ptr<Initializer> &getInitializer() const { return initializer_; }
    const bool hasInitializer() const { return initializer_ != nullptr; }

    // from direct declarator
    void addArraySize(arraySize arraySize) { arraySizes_.push_back(std::move(arraySize)); }
    void setFunction(bool isFunction) { isFunction_ = isFunction; }
    const bool isFunction() const { return isFunction_; }
    void setIsArray(bool isArray) { isArray_ = isArray; }
    const bool isArray() const { return isArray_; }
    void setDeclarator(std::unique_ptr<Declarator> declarator) { declarator_ = std::move(declarator); }
    void setInitializer(std::unique_ptr<Initializer> initializer) { initializer_ = std::move(initializer); }

private:
    std::string identifier_;
    std::unique_ptr<Initializer> initializer_; // optional
    std::unique_ptr<Declarator> declarator_;   // optional
    bool isPointer_ = false;
    bool isArray_ = false;
    std::vector<arraySize> arraySizes_; // optional
    bool isFunction_ = false;
};

class DeclatatorList : public ASTNode
{
public:
    DeclatatorList(std::vector<std::unique_ptr<Declarator>> declarators)
        : declarators_(std::move(declarators)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<Declarator>> &getDeclarators() const { return declarators_; }

private:
    std::vector<std::unique_ptr<Declarator>> declarators_;
};

class Declaration : public ASTNode
{
public:
    Declaration(std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers, std::unique_ptr<DeclatatorList> declaratorList)
        : declarationSpecifiers_(std::move(declarationSpecifiers)), declaratorList_(std::move(declaratorList)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    // DECLARATION_SPECIFIERS
    const std::unique_ptr<DeclarationSpecifiers> &getDeclarationSpecifiers() const { return declarationSpecifiers_; }
    const std::unique_ptr<DeclatatorList> &getDeclaratorList() const { return declaratorList_; }
    // INIT_DECLARATOR_LIST

private:
    std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers_;
    std::unique_ptr<DeclatatorList> declaratorList_;
};

class Initializer : public ASTNode
{
public:
    void accept(ASTVisitor &visitor) override = 0;
};

class ExpressionInitializer : public Initializer
{
public:
    ExpressionInitializer(std::unique_ptr<Expr> expr) : expr_(std::move(expr)) {}
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getExpr() const { return expr_.get(); }
    void setExpression(std::unique_ptr<Expr> expr) { expr_ = std::move(expr); }

private:
    std::unique_ptr<Expr> expr_;
};

// Derived class for brace initializers
class BraceInitializer : public Initializer
{
public:
    BraceInitializer(std::unique_ptr<InitializerList> initializerList)
        : initializerList_(std::move(initializerList)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    InitializerList *getInitializerList() const { return initializerList_.get(); }

private:
    std::unique_ptr<InitializerList> initializerList_;
};

class InitializerList : public ASTNode
{
public:
    InitializerList(std::vector<std::unique_ptr<Initializer>> InitializerList)
        : InitializerList_(std::move(InitializerList)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<Initializer>> &getInitializerList() const { return InitializerList_; }

    void addInitializer(std::unique_ptr<Initializer> initializer)
    {
        InitializerList_.push_back(std::move(initializer));
    }

private:
    std::vector<std::unique_ptr<Initializer>> InitializerList_;
};

class BlockItemBase : public Stmt
{
public:
    virtual ~BlockItemBase() = default;
    virtual void accept(ASTVisitor &visitor) = 0; // Interface for visiting
};

class BlockStmt : public Stmt
{
public:
    BlockStmt(std::vector<std::unique_ptr<BlockItemBase>> item) : item_(std::move(item)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<BlockItemBase>> &getItems() const { return item_; }

private:
    std::vector<std::unique_ptr<BlockItemBase>>
        item_;
};

class DeclarationWrapper : public BlockItemBase
{
public:
    DeclarationWrapper(std::unique_ptr<Declaration> declaration)
        : declaration_(std::move(declaration)) {}

    void accept(ASTVisitor &visitor) override
    {
        declaration_->accept(visitor);
    }

    Declaration *getDeclaration() const { return declaration_.get(); }

private:
    std::unique_ptr<Declaration> declaration_;
};

class StatementWrapper : public BlockItemBase
{
public:
    StatementWrapper(std::unique_ptr<Stmt> statement)
        : statement_(std::move(statement)) {}

    void accept(ASTVisitor &visitor) override
    {
        statement_->accept(visitor);
    }

    Stmt *getStatement() const { return statement_.get(); }

private:
    std::unique_ptr<Stmt> statement_;
};

class LabaledStatement : public Stmt
{
public:
    virtual ~LabaledStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class CaseStatement : public LabaledStatement
{
public:
    CaseStatement(std::unique_ptr<Expr> expr, std::unique_ptr<Stmt> statement)
        : expr_(std::move(expr)), statement_(std::move(statement)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getExpr() const { return expr_.get(); }
    Stmt *getStatement() const { return statement_.get(); }

private:
    std::unique_ptr<Expr> expr_;
    std::unique_ptr<Stmt> statement_;
};

class DefaultStatement : public LabaledStatement
{
public:
    DefaultStatement(std::unique_ptr<Stmt> statement)
        : statement_(std::move(statement)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Stmt *getStatement() const { return statement_.get(); }

private:
    std::unique_ptr<Stmt> statement_;
};

class ExpressionStatement : public Stmt
{
public:
    ExpressionStatement(std::unique_ptr<Expr> expr) : expr_(std::move(expr)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getExpr() const { return expr_.get(); }

private:
    std::unique_ptr<Expr> expr_;
};

class SelectionStatement : public Stmt
{
public:
    virtual ~SelectionStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class SwitchStatement : public SelectionStatement
{
public:
    SwitchStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition_(std::move(condition)), body_(std::move(body)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getCondition() const { return condition_.get(); }
    Stmt *getBody() const { return body_.get(); }

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> body_;
};

class IfStatement : public SelectionStatement
{
public:
    IfStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then)
        : condition_(std::move(condition)), then_(std::move(then)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getCondition() const { return condition_.get(); }
    Stmt *getThen() const { return then_.get(); }

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> then_;
};

class IterationStatement : public Stmt
{
public:
    virtual ~IterationStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ForInitStatement : public Stmt
{
public:
    virtual ~ForInitStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ForStatement : public IterationStatement
{
public:
    ForStatement(std::unique_ptr<ForInitStatement> init, std::unique_ptr<Stmt> statement)
        : init_(std::move(init)), statement_(std::move(statement)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    ForInitStatement *getInit() const { return init_.get(); }
    Stmt *getStatement() const { return statement_.get(); }

private:
    std::unique_ptr<ForInitStatement> init_;
    std::unique_ptr<Stmt> statement_;
};

class ForWitDeclaration : public ForInitStatement
{
public:
    ForWitDeclaration(std::unique_ptr<Declaration> declaration, std::unique_ptr<Expr> expr, std::unique_ptr<Expr> optionalExpr)
        : declaration_(std::move(declaration)), expr_(std::move(expr)), optionalExpr_(std::move(optionalExpr)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Declaration *getDeclaration() const { return declaration_.get(); }
    Expr *getExpr() const { return expr_.get(); }
    Expr *getOptionalExpr() const { return optionalExpr_.get(); }

private:
    std::unique_ptr<Declaration> declaration_;
    std::unique_ptr<Expr> expr_;
    std::unique_ptr<Expr> optionalExpr_;
};

class ForWithExpression : public ForInitStatement
{
public:
    ForWithExpression(std::unique_ptr<Expr> expr, std::unique_ptr<Expr> expr2, std::unique_ptr<Expr> optionalExpr)
        : expr_(std::move(expr)), expr2_(std::move(expr2)), optionalExpr_(std::move(optionalExpr)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getExpr() const { return expr_.get(); }
    Expr *getExpr2() const { return expr2_.get(); }
    Expr *getOptionalExpr() const { return optionalExpr_.get(); }

private:
    std::unique_ptr<Expr> expr_;
    std::unique_ptr<Expr> expr2_;
    std::unique_ptr<Expr> optionalExpr_;
};

class WhileStatement : public IterationStatement
{
public:
    WhileStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition_(std::move(condition)), body_(std::move(body)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getCondition() const { return condition_.get(); }
    Stmt *getBody() const { return body_.get(); }

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> body_;
};

class DoWhileStatement : public IterationStatement
{
public:
    DoWhileStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition_(std::move(condition)), body_(std::move(body)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getCondition() const { return condition_.get(); }
    Stmt *getBody() const { return body_.get(); }

private:
    std::unique_ptr<Expr> condition_;
    std::unique_ptr<Stmt> body_;
};

class JumpStatement : public Stmt
{
};

class ReturnStatement : public JumpStatement
{
public:
    ReturnStatement(std::unique_ptr<Expr> expr) : expr_(std::move(expr)) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getExpr() const { return expr_.get(); }

private:
    std::unique_ptr<Expr> expr_;
};

class ContinueStatement : public JumpStatement
{
public:
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
};

class BreakStatement : public JumpStatement
{
public:
    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
};

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
};
