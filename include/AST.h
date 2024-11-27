#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Forward declarations for visitor pattern
class Program;
class BinaryExpr;
class FunctionDecl;
class Declaration;
class BlockStmt;
class PrimitiveType;
class Specifier;
class DeclarationSpecifiers;
class Declarator;
class DeclaratorList;
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
class ConditionalExpression;
class ParenthesizedExpression;
class ConstantExpression;
class IdentifierExpression;
class PrimaryExpression;
class ArrayPostFixExpression;
class AssignmentExpressionList;
class AssignmentOperator;
class AssignmentExpression;
class ArgumentsPostFixExpression;
class DotOperatorPostfixExpression;
class ArrowOperatorPostfixExpression;
class IncrementDecrementPostfixExpression;
class PostfixExpressions;
class UnaryIncrementDecrementOperator;
class ArrayDirectDeclarator;
class FunctionDirectDeclarator;
class IdentifierList;
class ParameterDeclaration;
class ParameterDeclarationList;

class AnonimousStruct;
class StructDeclaration;
class StructMemberDeclarationList;
class StructMemberDeclaration;

#include "ASTVisitor.h"

// Abstract base class for all AST nodes
class ASTNode
{
public:
    ASTNode(int line) : line_(line) {}  
    virtual ~ASTNode() = default;

    virtual void accept(ASTVisitor &visitor) = 0;

    int getLine() const { return line_; }

protected: 
    int line_;
};

// Program node as the root of the AST
class Program : public ASTNode
{
public:
    Program(std::vector<std::unique_ptr<ASTNode>> topLevelDeclarations, int line)
        : topLevelDeclarations_(std::move(topLevelDeclarations)), ASTNode(line) {}

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
public:
    Expr(int line) : ASTNode(line) {}
};

// Statement node base class
class Stmt : public ASTNode
{
public:
    Stmt(int line) : ASTNode(line) {}
};

class BinaryExpr : public Expr
{
public:
    BinaryExpr(std::unique_ptr<Expr> lhs, std::string op, std::unique_ptr<Expr> rhs, int line)
        : lhs_(std::move(lhs)), op_(op), rhs_(std::move(rhs)), Expr(line) {}

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

// Function Declaration Node
class FunctionDecl : public ASTNode
{
public:
    FunctionDecl(std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers, std::unique_ptr<Declarator> declarator, std::unique_ptr<BlockStmt> block, int line)
        : declarationSpecifiers_(std::move(declarationSpecifiers)), declarator_(std::move(declarator)), block_(std::move(block)), ASTNode(line) {}

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
    Type(int line) : ASTNode(line) {}
    virtual ~Type() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class PrimitiveType : public Type
{
public:
    PrimitiveType(std::string baseType, int line)
        : baseType_(std::move(baseType)), Type(line) {}

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
    explicit Specifier(std::string specifier, std::string specifierType, int line) 
        : specifier_(std::move(specifier)), specifierType_(std::move(specifierType)), ASTNode(line) {}

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
    DeclarationSpecifiers(std::unique_ptr<Type> type, std::unique_ptr<Specifier> specifiers, int line)
        : type_(std::move(type)), specifier_(std::move(specifiers)), ASTNode(line) {}

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

class DirectDeclarator : public ASTNode
{
public:
    DirectDeclarator (int line) : ASTNode(line) {}
    virtual ~DirectDeclarator() = default;

    // Accept method for the visitor
    virtual void accept(ASTVisitor &visitor) = 0;
};

class Declarator : public ASTNode
{
public:
    Declarator(int line): ASTNode(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
    // optional pointer
    const void setPointer(bool isPointer) { isPointer_ = isPointer; }
    const bool isPointer() const { return isPointer_; }

    const void setIdentifier(std::string identifier) { identifier_ = identifier; }
    const std::string &getIdentifier() const { return identifier_; }

    // optional dclarator
    void setDeclarator(std::unique_ptr<Declarator> declarator) { declarator_ = std::move(declarator); }
    const std::unique_ptr<Declarator> &getDeclarator() const { return declarator_; }

    // optional initializer for INIT_DECLARATOR
    void setInitializer(std::unique_ptr<Initializer> initializer) { initializer_ = std::move(initializer); }
    const std::unique_ptr<Initializer> &getInitializer() const { return initializer_; }

    // from direct declarator
    void addDirectDeclarator(std::unique_ptr<DirectDeclarator> directDeclarator) { directDeclarators.push_back(std::move(directDeclarator)); }
    const std::vector<std::unique_ptr<DirectDeclarator>> &getDirectDeclarators() const { return directDeclarators; }

private:
    std::string identifier_;
    std::unique_ptr<Initializer> initializer_; // optional
    std::unique_ptr<Declarator> declarator_;   // optional
    bool isPointer_ = false;
    std::vector<std::unique_ptr<DirectDeclarator>> directDeclarators;
};

class ArrayDirectDeclarator : public DirectDeclarator
{
public:
    ArrayDirectDeclarator(std::unique_ptr<Expr> ArrSize, std::unique_ptr<Specifier> typeQualifier, bool isStatic, int line)
        : ArrSize(std::move(ArrSize)), typeQualifier(std::move(typeQualifier)), isStatic(isStatic), DirectDeclarator(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
    Expr *getArrSize() const { return ArrSize.get(); }
    Specifier *getTypeQualifier() const { return typeQualifier.get(); }
    bool getIsStatic() const { return isStatic; }

private:
    bool isStatic;
    std::unique_ptr<Specifier> typeQualifier;
    std::unique_ptr<Expr> ArrSize;
};

class FunctionDirectDeclarator : public DirectDeclarator
{
public:
    FunctionDirectDeclarator(int line) : DirectDeclarator(line) {}
    virtual ~FunctionDirectDeclarator() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class IdentifierList : public FunctionDirectDeclarator
{
public:
    IdentifierList(std::vector<std::string> identifiers, int line)
        : identifiers_(std::move(identifiers)) , FunctionDirectDeclarator(line){}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::string> &getIdentifiers() const { return identifiers_; }

private:
    std::vector<std::string> identifiers_;
};

class ParameterDeclaration : public ASTNode
{
public:
    ParameterDeclaration(std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers, std::unique_ptr<Declarator> declarator, int line)
        : declarationSpecifiers_(std::move(declarationSpecifiers)), declarator_(std::move(declarator)), ASTNode(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::unique_ptr<DeclarationSpecifiers> &getDeclarationSpecifiers() const { return declarationSpecifiers_; }
    const std::unique_ptr<Declarator> &getDeclarator() const { return declarator_; }

private:
    std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers_;
    std::unique_ptr<Declarator> declarator_;
};

class ParameterDeclarationList : public FunctionDirectDeclarator
{
public:
    ParameterDeclarationList(std::vector<std::unique_ptr<ParameterDeclaration>> parameters, int line)
        : parameters_(std::move(parameters)) , FunctionDirectDeclarator(line){}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<ParameterDeclaration>> &getParameters() const { return parameters_; }

private:
    std::vector<std::unique_ptr<ParameterDeclaration>> parameters_;
};

class DeclaratorList : public ASTNode
{
public:
    DeclaratorList(std::vector<std::unique_ptr<Declarator>> declarators, int line)
        : declarators_(std::move(declarators)), ASTNode(line) {}

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
    Declaration(std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers, std::unique_ptr<DeclaratorList> declaratorList, int line)
        : declarationSpecifiers_(std::move(declarationSpecifiers)), declaratorList_(std::move(declaratorList)), ASTNode(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    // DECLARATION_SPECIFIERS
    const std::unique_ptr<DeclarationSpecifiers> &getDeclarationSpecifiers() const { return declarationSpecifiers_; }
    const std::unique_ptr<DeclaratorList> &getDeclaratorList() const { return declaratorList_; }
    // INIT_DECLARATOR_LIST

private:
    std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers_;
    std::unique_ptr<DeclaratorList> declaratorList_;
};

class Initializer : public ASTNode
{
public:
    Initializer(int line) : ASTNode(line) {}
    void accept(ASTVisitor &visitor) override = 0;
};

class ExpressionInitializer : public Initializer
{
public:
    ExpressionInitializer(std::unique_ptr<Expr> expr, int line) : expr_(std::move(expr)), Initializer(line){}
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
    BraceInitializer(std::unique_ptr<InitializerList> initializerList, int line)
        : initializerList_(std::move(initializerList)), Initializer(line) {}

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
    InitializerList(std::vector<std::unique_ptr<Initializer>> InitializerList, int line)
        : InitializerList_(std::move(InitializerList)), ASTNode(line) {}

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
    BlockItemBase(int line) : Stmt(line) {}
    virtual ~BlockItemBase() = default;
    virtual void accept(ASTVisitor &visitor) = 0; // Interface for visiting
};

class BlockStmt : public Stmt
{
public:
    BlockStmt(std::vector<std::unique_ptr<BlockItemBase>> item, int line) : item_(std::move(item)), Stmt(line) {}

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
    DeclarationWrapper(std::unique_ptr<Declaration> declaration, int line)
        : declaration_(std::move(declaration)), BlockItemBase(line) {}

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
    StatementWrapper(std::unique_ptr<Stmt> statement, int line)
        : statement_(std::move(statement)), BlockItemBase(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Stmt *getStatement() const { return statement_.get(); }

private:
    std::unique_ptr<Stmt> statement_;
};

class LabaledStatement : public Stmt
{
public:
    LabaledStatement(int line) : Stmt(line) {}
    virtual ~LabaledStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class CaseStatement : public LabaledStatement
{
public:
    CaseStatement(std::unique_ptr<Expr> expr, std::unique_ptr<Stmt> statement, int line)
        : expr_(std::move(expr)), statement_(std::move(statement)), LabaledStatement(line) {}

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
    DefaultStatement(std::unique_ptr<Stmt> statement, int line)
        : statement_(std::move(statement)), LabaledStatement(line) {}

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
    ExpressionStatement(std::unique_ptr<Expr> expr, int line) : expr_(std::move(expr)), Stmt(line) {}

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
    SelectionStatement(int line) : Stmt(line) {}
    virtual ~SelectionStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class SwitchStatement : public SelectionStatement
{
public:
    SwitchStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body, int line)
        : condition_(std::move(condition)), body_(std::move(body)), SelectionStatement(line) {}

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
    IfStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then, int line)
        : condition_(std::move(condition)), then_(std::move(then)), SelectionStatement(line) {}

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
    IterationStatement(int line): Stmt(line) {}
    virtual ~IterationStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ForInitStatement : public Stmt
{
public:
    ForInitStatement(int line): Stmt(line) {}
    virtual ~ForInitStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ForStatement : public IterationStatement
{
public:
    ForStatement(std::unique_ptr<ForInitStatement> init, std::unique_ptr<Stmt> statement, int line)
        : init_(std::move(init)), statement_(std::move(statement)), IterationStatement (line){}

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
    ForWitDeclaration(std::unique_ptr<Declaration> declaration, std::unique_ptr<Expr> expr, std::unique_ptr<Expr> optionalExpr, int line)
        : declaration_(std::move(declaration)), expr_(std::move(expr)), optionalExpr_(std::move(optionalExpr)), ForInitStatement (line){}

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
    ForWithExpression(std::unique_ptr<Expr> expr, std::unique_ptr<Expr> expr2, std::unique_ptr<Expr> optionalExpr, int line)
        : expr_(std::move(expr)), expr2_(std::move(expr2)), optionalExpr_(std::move(optionalExpr)), ForInitStatement(line) {}

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
    WhileStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body, int line)
        : condition_(std::move(condition)), body_(std::move(body)), IterationStatement (line){}

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
    DoWhileStatement(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body, int line)
        : condition_(std::move(condition)), body_(std::move(body)), IterationStatement(line) {}

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
public:
    JumpStatement(int line) : Stmt(line) {}
    virtual ~JumpStatement() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ReturnStatement : public JumpStatement
{
public:
    ReturnStatement(std::unique_ptr<Expr> expr, int line) : expr_(std::move(expr)), JumpStatement(line) {}

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
    ContinueStatement(int line) : JumpStatement(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
};

class BreakStatement : public JumpStatement
{
public:
    BreakStatement(int line) : JumpStatement(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }
};

class ConditionalExpression : public Expr
{
public:
    ConditionalExpression(std::unique_ptr<Expr> logicalOrExpression, std::unique_ptr<Expr> expression, std::unique_ptr<ConditionalExpression> conditionalExpression, int line)
        : logicalOrExpression(std::move(logicalOrExpression)), expression(std::move(expression)), conditionalExpression(std::move(conditionalExpression)), Expr(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getLogicalOrExpression() const { return logicalOrExpression.get(); }
    Expr *getExpression() const { return expression.get(); }
    ConditionalExpression *getConditionalExpression() const { return conditionalExpression.get(); }

private:
    std::unique_ptr<Expr> logicalOrExpression;
    std::unique_ptr<Expr> expression;                             // optional
    std::unique_ptr<ConditionalExpression> conditionalExpression; // optional
};

class PrimaryExpression : public Expr
{
public:
    PrimaryExpression(int line) : Expr(line) {}
    virtual ~PrimaryExpression() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class IdentifierExpression : public PrimaryExpression
{
public:
    IdentifierExpression(std::string identifier, int line) : identifier(identifier), PrimaryExpression(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getIdentifier() const { return identifier; }

private:
    std::string identifier;
};

enum ConstantType
{
    INTEGER_CONSTANT,
    FLOAT_CONSTANT,
    STRING_LITERAL
};

class ConstantExpression : public PrimaryExpression
{
public:
    ConstantExpression(std::string constant, ConstantType type, int line) : constant(constant), constantType(type), PrimaryExpression(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getConstant() const { return constant; }
    const ConstantType &getConstantType() const { return constantType; }
    std::string getConstantTypeString() const
    {
        switch (constantType)
        {
        case INTEGER_CONSTANT:
            return "INTEGER_CONSTANT";
        case FLOAT_CONSTANT:
            return "FLOAT_CONSTANT";
        case STRING_LITERAL:
            return "STRING_LITERAL";
        default:
            return "UNKNOWN_CONSTANT";
        }
    }

private:
    ConstantType constantType;
    std::string constant;
};

class ParenthesizedExpression : public PrimaryExpression
{
public:
    ParenthesizedExpression(std::unique_ptr<Expr> expr, int line) : expr(std::move(expr)), PrimaryExpression(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getExpr() const { return expr.get(); }

private:
    std::unique_ptr<Expr> expr;
};

class PostfixExpressionBase : public Expr
{
public:
    PostfixExpressionBase(int line) : Expr(line) {}
    virtual ~PostfixExpressionBase() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ArrayPostFixExpression : public PostfixExpressionBase
{
public:
    ArrayPostFixExpression(std::unique_ptr<Expr> expr, int line) : ArraySize(std::move(expr)), PostfixExpressionBase(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Expr *getArraySize() const { return ArraySize.get(); }

private:
    std::unique_ptr<Expr> ArraySize;
};

class AssignmentExpression : public Expr
{
public:
    AssignmentExpression(std::unique_ptr<ConditionalExpression> conditionalExpression, std::unique_ptr<AssignmentExpression> assigmentExpression, std::unique_ptr<AssignmentOperator> assigmentOperator, int line)
        : conditionalExpression(std::move(conditionalExpression)), assigmentExpression(std::move(assigmentExpression)), assigmentOperator(std::move(assigmentOperator)), Expr(line){}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    ConditionalExpression *getConditionalExpression() const { return conditionalExpression.get(); }
    AssignmentExpression *getAssignmentExpression() const { return assigmentExpression.get(); }
    AssignmentOperator *getAssignmentOperator() const { return assigmentOperator.get(); }

private:
    std::unique_ptr<ConditionalExpression> conditionalExpression;
    std::unique_ptr<AssignmentExpression> assigmentExpression; // optional
    std::unique_ptr<AssignmentOperator> assigmentOperator;     // optional
};

class AssignmentOperator : public Expr
{
public:
    AssignmentOperator(std::string op, int line)
        : op(std::move(op)), Expr(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getOp() const { return op; }

private:
    std::string op;
};

class AssignmentExpressionList : public Expr
{
public:
    AssignmentExpressionList(std::vector<std::unique_ptr<AssignmentExpression>> assigmentExpression, int line)
        : assigmentExpression(std::move(assigmentExpression)), Expr(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<AssignmentExpression>> &getAssignmentExpression() const
    {
        return assigmentExpression;
    }

    void addAssignmentExpression(std::unique_ptr<AssignmentExpression> expr)
    {
        assigmentExpression.push_back(std::move(expr));
    }

private:
    std::vector<std::unique_ptr<AssignmentExpression>> assigmentExpression;
};

class ArgumentsPostFixExpression : public PostfixExpressionBase
{
public:
    ArgumentsPostFixExpression(std::unique_ptr<AssignmentExpressionList> argumentList, int line)
        : argumentList(std::move(argumentList)), PostfixExpressionBase(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    AssignmentExpressionList *getArgumentList() const { return argumentList.get(); }

private:
    std::unique_ptr<AssignmentExpressionList> argumentList;
};

class DotOperatorPostfixExpression : public PostfixExpressionBase
{
public:
    DotOperatorPostfixExpression(std::string identifier, int line) : identifier(identifier), PostfixExpressionBase(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getIdentifier() const { return identifier; }

private:
    std::string identifier;
};

class ArrowOperatorPostfixExpression : public PostfixExpressionBase
{
public:
    ArrowOperatorPostfixExpression(std::string identifier, int line) : identifier(identifier), PostfixExpressionBase(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getIdentifier() const { return identifier; }

private:
    std::string identifier;
};

class IncrementDecrementPostfixExpression : public PostfixExpressionBase
{
public:
    IncrementDecrementPostfixExpression(std::string op, int line) : op(op), PostfixExpressionBase (line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::string &getOp() const { return op; }

private:
    std::string op;
};

class UnaryExpr : public Expr
{
public:
    UnaryExpr(int line) : Expr(line) {}
    virtual ~UnaryExpr() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class PostfixExpressions : public UnaryExpr
{
public:
    PostfixExpressions(std::unique_ptr<PrimaryExpression> primaryExpression, std::vector<std::unique_ptr<PostfixExpressionBase>> postfixExpression, int line)
        : primaryExpression(std::move(primaryExpression)), postfixExpression(std::move(postfixExpression)), UnaryExpr(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<PostfixExpressionBase>> &getPostfixExpressions() const
    {
        return postfixExpression;
    }

    PrimaryExpression *getPrimaryExpression() const { return primaryExpression.get(); }

    void addPostfixExpression(std::unique_ptr<PostfixExpressionBase> expr)
    {
        postfixExpression.push_back(std::move(expr));
    }

private:
    std::vector<std::unique_ptr<PostfixExpressionBase>> postfixExpression;
    std::unique_ptr<PrimaryExpression> primaryExpression;
};

class UnaryIncrementDecrementOperator : public UnaryExpr
{
public:
    UnaryIncrementDecrementOperator(std::string op, std::unique_ptr<UnaryExpr> unaryExpr, int line)
        : op(op), unaryExpr(std::move(unaryExpr)), UnaryExpr(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    UnaryExpr *getUnaryExpr() const { return unaryExpr.get(); }

    const std::string &getOp() const { return op; }

private:
    std::string op;
    std::unique_ptr<UnaryExpr> unaryExpr;
};

class StructSpecifier : public Type
{
public:
    StructSpecifier(int line) : Type(line) {}
    virtual ~StructSpecifier() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class AnonimousStruct : public StructSpecifier
{
public:
    AnonimousStruct(std::unique_ptr<StructMemberDeclarationList> structDeclarationList, int line)
        : structMemberDeclarationList(std::move(structDeclarationList)), StructSpecifier(line){}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    StructMemberDeclarationList *getStructDeclarationList() const { return structMemberDeclarationList.get(); }

private:
    std::unique_ptr<StructMemberDeclarationList> structMemberDeclarationList;
};

class StructDeclaration : public StructSpecifier
{
public:
    StructDeclaration(std::unique_ptr<StructMemberDeclarationList> structDeclarationList, std::string identifier, int line)
        : structMemberDeclarationList(std::move(structDeclarationList)), identifier(identifier), StructSpecifier(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    StructMemberDeclarationList *getStructDeclarationList() const { return structMemberDeclarationList.get(); }
    const std::string &getIdentifier() const { return identifier; }

private:
    std::string identifier;
    std::unique_ptr<StructMemberDeclarationList> structMemberDeclarationList;
};

class StructMemberDeclarationList : public ASTNode
{
public:
    StructMemberDeclarationList(std::vector<std::unique_ptr<StructMemberDeclaration>> structMemberDeclarations, int line)
        : structMemberDeclarations(std::move(structMemberDeclarations)), ASTNode(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    const std::vector<std::unique_ptr<StructMemberDeclaration>> &getStructMemberDeclarations() const
    {
        return structMemberDeclarations;
    }

private:
    std::vector<std::unique_ptr<StructMemberDeclaration>> structMemberDeclarations;
};

class StructMemberDeclaration : public ASTNode
{
public:
    StructMemberDeclaration(std::unique_ptr<Type> type, std::unique_ptr<DeclaratorList> declaratorList, int line)
        : typeSpecifier(std::move(type)), declaratorList(std::move(declaratorList)), ASTNode(line) {}

    void accept(ASTVisitor &visitor) override
    {
        visitor.visit(*this);
    }

    Type *getTypeSpecifier() const { return typeSpecifier.get(); }
    DeclaratorList *getDeclaratorList() const { return declaratorList.get(); }

private:
    std::unique_ptr<Type> typeSpecifier;
    std::unique_ptr<DeclaratorList> declaratorList;
};
