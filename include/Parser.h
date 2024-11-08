#pragma once
#include "AST.h"
#include <string>
#include <memory>
#include <vector>
#include "TokenTable.h"

class Parser
{
public:
    Parser(TokenTable &tokenTable) : tokens(tokenTable)
    {
        currentToken = tokens.front();
    }

    std::unique_ptr<Program> parsePROGRAM(); // this is the starting point

private:
    TokenTable &tokens;
    Token currentToken;

    void consume();
    void expect(TokenType expected); // consume el token si es del tipo que queremos

    bool isExpressionFirst() const;
    bool isDeclarationFirst() const;
    bool isBasicType() const;
    bool isBlockFirst() const;
    bool isStatementFirst() const;
    // Expression
    std::unique_ptr<Expr> parseAdditveExpression();
    std::unique_ptr<Expr> parseAndExpression();
    std::unique_ptr<AssignmentExpressionList> parseArgumentExpressionList();
    std::unique_ptr<AssignmentExpression> parseAssignmentExpression();
    std::unique_ptr<AssignmentExpression> parseAssignmentExpressionOptFac();
    std::unique_ptr<AssignmentOperator> parseAssignmentOperator();
    std::unique_ptr<BlockStmt> parseBlock();
    std::unique_ptr<BlockStmt> parseBlockContent();
    std::unique_ptr<BlockItemBase> parseBlockItem();
    std::unique_ptr<BlockStmt> parseBlockItemList();
    std::unique_ptr<ConditionalExpression> parseConditionalExpression();
    std::unique_ptr<ConstantExpression> parseConstant();
    std::unique_ptr<Declaration> parseDeclaration();
    std::unique_ptr<DeclaratorList> parseDeclarationFac();
    std::unique_ptr<ASTNode> parseDeclarationList();
    std::unique_ptr<ASTNode> parseDeclarationListPrime();
    std::unique_ptr<DeclarationSpecifiers> parseDeclarationSpecifiers();
    std::unique_ptr<Specifier> parseDeclarationSpecifiersPrime();
    std::unique_ptr<Declarator> parseDeclarator();
    void parseDirectDeclarator(std::unique_ptr<Declarator> &declarator);
    void parseDirectDeclaratorPrime(std::unique_ptr<Declarator> &declarato);
    std::unique_ptr<Expr> parseEqualityExpression();
    std::unique_ptr<Expr> parseExclusiveOrExpression();
    std::unique_ptr<Expr> parseEXPRESSION();
    std::unique_ptr<Expr> parseExpressionStatement();
    std::unique_ptr<ForInitStatement> parseForInitStatement();
    std::unique_ptr<Expr> parseForOptionalExpression();
    std::unique_ptr<FunctionDecl> parseFunctionDefinition();
    std::unique_ptr<BlockStmt> parseFunctionDefFac();
    std::unique_ptr<Specifier> parseFunctionSpecifier();
    std::unique_ptr<ASTNode> parseGlobalDeclarations();
    std::unique_ptr<IdentifierList> parseIdentifierList();
    std::unique_ptr<Expr> parseInclusiveOrExpression();
    std::unique_ptr<Initializer> parseInitializer();
    std::unique_ptr<InitializerList> parseInitializerBraceFac();
    std::unique_ptr<InitializerList> parseInitializerList();
    std::unique_ptr<Declarator> parseInitDeclarator();
    std::unique_ptr<Initializer> parseInitDeclaratorFac();
    std::unique_ptr<DeclaratorList> parseInitDeclaratorList();
    std::unique_ptr<IterationStatement> parseIterationStatement();
    std::unique_ptr<JumpStatement> parseJumpStatement();
    std::unique_ptr<LabaledStatement> parseLabeledStatement();
    std::unique_ptr<Expr> parseLogicalAndExpression();
    std::unique_ptr<Expr> parseLogicalOrExpression();
    std::unique_ptr<Expr> parseMultiplicativeExpression();
    std::unique_ptr<ParameterDeclaration> parseParameterDeclaration();
    std::unique_ptr<ParameterDeclarationList> parseParameterList();
    std::unique_ptr<FunctionDirectDeclarator> parseParameterListOptFac();
    std::unique_ptr<Declarator> parseParamDeclFac();
    bool parsePointer();
    std::unique_ptr<AssignmentExpressionList> parsePostfixArguments();
    std::unique_ptr<PostfixExpressions> parsePostfixExpression();
    std::unique_ptr<PrimaryExpression> parsePrimaryExpression();
    std::unique_ptr<Expr> parseRelationalExpression();
    std::unique_ptr<SelectionStatement> parseSelectionStatement();
    std::unique_ptr<Expr> parseShiftExpression();
    std::unique_ptr<ASTNode> parseSPECIFIER_QUALIFIER_LIST();
    std::unique_ptr<Stmt> parseStatement();
    bool parseStaticOptFac();
    std::unique_ptr<Specifier> parseStorageSpecifier();
    std::unique_ptr<StructMemberDeclaration> parseSTRUCT_DECLARATION();
    std::unique_ptr<StructMemberDeclarationList> parseSTRUCT_DECLARATION_LIST();
    std::unique_ptr<DeclaratorList> parseSTRUCT_DECLARATOR_LIST();
    std::unique_ptr<DeclaratorList> parseSTRUCT_DECL_FAC();
    std::unique_ptr<StructSpecifier> parseSTRUCT_SPECIFIER();
    std::unique_ptr<StructSpecifier> parseSTRUCT_SPEC_FAC();
    std::unique_ptr<StructMemberDeclarationList> parseSTRUCT_SPEC_FAC2();
    std::unique_ptr<Specifier> parseTypeQualifier();
    std::unique_ptr<Specifier> parseTypeQualifierListOptFac();
    std::unique_ptr<Type> parseTypeSpecifier();
    std::unique_ptr<UnaryExpr> parseUnaryExpression();
};
