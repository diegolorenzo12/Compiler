#include <stdexcept>
#include "ErrorManager.h"
#include "Parser.h"
#include "PrintVisitor.h"
/*
HELPER FUNCTIONS
*/

bool Parser::isBasicType() const
{
    TokenType tokenType = currentToken.getType();
    return tokenType == TokenType::BOOL ||
           tokenType == TokenType::CHAR ||
           tokenType == TokenType::DOUBLE ||
           tokenType == TokenType::FLOAT ||
           tokenType == TokenType::INT ||
           tokenType == TokenType::LONG ||
           tokenType == TokenType::SHORT ||
           tokenType == TokenType::SIGNED ||
           tokenType == TokenType::STRUCT ||
           tokenType == TokenType::UNSIGNED ||
           tokenType == TokenType::VOID;
}

// ( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
bool Parser::isExpressionFirst() const
{
    const std::string &tokenValue = currentToken.getValue();

    return (currentToken.getType() == TokenType::LPAREN) ||
           (currentToken.getType() == TokenType::FLOAT_CONSTANT) ||
           (currentToken.getType() == TokenType::INTEGER_CONSTANT) ||
           (currentToken.getType() == TokenType::IDENTIFIER) ||
           (currentToken.getType() == TokenType::STRING_LITERAL) ||
           (currentToken.getType() == TokenType::INC_OP) ||
           (currentToken.getType() == TokenType::DEC_OP);
}

//  func bool char double float int long short signed struct unsigned void
bool Parser::isDeclarationFirst() const
{
    const std::string &tokenValue = currentToken.getValue();

    return isBasicType() || (currentToken.getType() == TokenType::FUNC);
}

//       ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL bool break case char continue default do double float for if int long return short signed sizeof struct switch unsigned void while {
bool Parser::isBlockFirst() const
{
    return isBasicType() || isStatementFirst();
}

//       ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL break case continue default do for if return sizeof switch while {
bool Parser::isStatementFirst() const
{
    TokenType tokenType = currentToken.getType();

    return tokenType == TokenType::INC_OP ||
           tokenType == TokenType::DEC_OP ||
           tokenType == TokenType::FLOAT_CONSTANT ||
           tokenType == TokenType::INTEGER_CONSTANT ||
           tokenType == TokenType::IDENTIFIER ||
           tokenType == TokenType::STRING_LITERAL ||
           tokenType == TokenType::LBRACE ||
           tokenType == TokenType::SEMICOLON ||
           tokenType == TokenType::LPAREN ||
           tokenType == TokenType::BREAK ||
           tokenType == TokenType::CASE ||
           tokenType == TokenType::CONTINUE ||
           tokenType == TokenType::DEFAULT ||
           tokenType == TokenType::DO ||
           tokenType == TokenType::FOR ||
           tokenType == TokenType::IF ||
           tokenType == TokenType::RETURN ||
           tokenType == TokenType::SIZEOF ||
           tokenType == TokenType::SWITCH ||
           tokenType == TokenType::WHILE;
}

void Parser::consume()
{
    tokens.pop_front();
    this->currentToken = tokens.front();
}

void Parser::expect(TokenType expected)
{
    if (currentToken.getType() != expected)
    {
        throw SyntacticException(currentToken.getLineNumber(), "Unexpected token: " + currentToken.getValue());
    }
    consume();
}

/*
PARSER
*/

// PROGRAM -> GLOBAL_DECLARATIONS PROGRAM_PRIME
std::unique_ptr<Program> Parser::parsePROGRAM()
{
    std::vector<std::unique_ptr<ASTNode>> topLevelDeclarations;

    topLevelDeclarations.push_back(parseGlobalDeclarations());

    // PROGRAM_PRIME -> GLOBAL_DECLARATIONS PROGRAM_PRIME | ϵ
    while (isDeclarationFirst())
    {
        topLevelDeclarations.push_back(parseGlobalDeclarations());
    }

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw SyntacticException(currentToken.getLineNumber(), "Unexpected token: " + currentToken.getValue());
    }
    std::unique_ptr<Program> program = std::make_unique<Program>(std::move(topLevelDeclarations), currentToken.getLineNumber());

    return program;
}

// GLOBAL_DECLARATIONS -> DECLARATION | FUNCTION_DEFINITION
std::unique_ptr<ASTNode> Parser::parseGlobalDeclarations()
{
    if (currentToken.getType() == TokenType::FUNC)
    {
        std::unique_ptr<FunctionDecl> functionDecl = parseFunctionDefinition();
        return functionDecl;
    }
    else if (isBasicType())
    {
        std::unique_ptr<Declaration> declaration = parseDeclaration();
        return declaration;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected declaration or function definition");
    }
}

// DECLARATION -> DECLARATION_SPECIFIERS DECLARATION_FAC
std::unique_ptr<Declaration> Parser::parseDeclaration()
{
    std::unique_ptr<DeclarationSpecifiers> DeclarationSpec = parseDeclarationSpecifiers();
    std::unique_ptr<DeclaratorList> declaratorList = parseDeclarationFac();
    return std::make_unique<Declaration>(std::move(DeclarationSpec), std::move(declaratorList), currentToken.getLineNumber());
}

// FIRST(INIT_DECLARATOR_LIST)= 	( * IDENTIFIER
// DECLARATION_FAC -> ; | INIT_DECLARATOR_LIST ;
std::unique_ptr<DeclaratorList> Parser::parseDeclarationFac()
{
    if (currentToken.getType() == TokenType::SEMICOLON)
    {
        consume();
        // empty declaration, example: int;
        return nullptr;
    }
    else if (currentToken.getType() == TokenType::LPAREN || currentToken.getType() == TokenType::ASTERISK || currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::unique_ptr<DeclaratorList> declaratorList = parseInitDeclaratorList();
        if (currentToken.getType() != TokenType::SEMICOLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ; at the end of declaration");
        }
        consume();
        return declaratorList;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected ; or INIT_DECLARATOR_LIST, instead got: " + currentToken.getValue());
    }
}

// ARGUMENT_EXPRESSION_LIST -> ASSIGNMENT_EXPRESSION ARGUMENT_EXPRESSION_LIST_PRIME
std::unique_ptr<AssignmentExpressionList> Parser::parseArgumentExpressionList()
{
    std::unique_ptr<AssignmentExpression> assigmentExpression = parseAssignmentExpression();
    std::vector<std::unique_ptr<AssignmentExpression>> expressionList;
    expressionList.push_back(std::move(assigmentExpression));

    // ARGUMENT_EXPRESSION_LIST_PRIME -> , ASSIGNMENT_EXPRESSION ARGUMENT_EXPRESSION_LIST_PRIME | ϵ

    while (currentToken.getType() == TokenType::COMMA)
    {
        consume();
        expressionList.push_back(parseAssignmentExpression());
    }
    return std::make_unique<AssignmentExpressionList>(std::move(expressionList), currentToken.getLineNumber());
}

// ASSIGNMENT_EXPRESSION -> CONDITIONAL_EXPRESSION ASSIGNMENT_EXPRESSION_FAC
std::unique_ptr<AssignmentExpression> Parser::parseAssignmentExpression()
{
    std::unique_ptr<ConditionalExpression> conditionalExpression = parseConditionalExpression();
    // ASSIGNMENT_EXPRESSION_FAC -> ASSIGNMENT_OPERATOR ASSIGNMENT_EXPRESSION | ϵ
    TokenType type = currentToken.getType();
    if (type == TokenType::EQUAL || type == TokenType::MUL_ASSIGN || type == TokenType::DIV_ASSIGN || type == TokenType::MOD_ASSIGN || type == TokenType::ADD_ASSIGN || type == TokenType::SUB_ASSIGN || type == TokenType::LEFT_ASSIGN || type == TokenType::RIGHT_ASSIGN || type == TokenType::AND_ASSIGN || type == TokenType::XOR_ASSIGN || type == TokenType::OR_ASSIGN)
    {
        std::unique_ptr<AssignmentOperator> assigmentOperator = parseAssignmentOperator();
        std::unique_ptr<AssignmentExpression> assigmentExpression = parseAssignmentExpression();
        return std::make_unique<AssignmentExpression>(std::move(conditionalExpression), std::move(assigmentExpression), std::move(assigmentOperator), currentToken.getLineNumber());
    }
    else
    {
        return std::make_unique<AssignmentExpression>(std::move(conditionalExpression), nullptr, nullptr, currentToken.getLineNumber());
    }
}

// ASSIGNMENT_EXPRESSION_OPT_FAC -> ASSIGNMENT_EXPRESSION | ϵ
// First(ASSIGNMENT_EXPRESSION_OPT_FAC) = {( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof ϵ}
std::unique_ptr<AssignmentExpression> Parser::parseAssignmentExpressionOptFac()
{
    if (isExpressionFirst())
    {
        return parseAssignmentExpression();
    }
    return nullptr;
}

// ASSIGNMENT_OPERATOR -> = | *= | /= | %= | += | -= | <<= | >>= | &&= | ^= | |=
std::unique_ptr<AssignmentOperator> Parser::parseAssignmentOperator()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::EQUAL || type == TokenType::MUL_ASSIGN || type == TokenType::DIV_ASSIGN || type == TokenType::MOD_ASSIGN || type == TokenType::ADD_ASSIGN || type == TokenType::SUB_ASSIGN || type == TokenType::LEFT_ASSIGN || type == TokenType::RIGHT_ASSIGN || type == TokenType::AND_ASSIGN || type == TokenType::XOR_ASSIGN || type == TokenType::OR_ASSIGN)
    {
        std::unique_ptr<AssignmentOperator> assigmentOperator = std::make_unique<AssignmentOperator>(currentToken.getValue(), currentToken.getLineNumber());
        consume();
        return assigmentOperator;
    }
    return nullptr;
}

// BLOCK -> { BLOCK_CONTENT }
std::unique_ptr<BlockStmt> Parser::parseBlock()
{

    if (currentToken.getType() == TokenType::LBRACE)
    {
        consume();
        std::unique_ptr<BlockStmt> block = parseBlockContent();
        if (currentToken.getType() == TokenType::RBRACE)
        {
            consume();
            return block;
        }
        else
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected '}' at the end of block");
        }
    }

    return nullptr;
}

// BLOCK_CONTENT -> BLOCK_ITEM_LIST | ϵ
std::unique_ptr<BlockStmt> Parser::parseBlockContent()
{
    if (isBlockFirst())
    {
        return parseBlockItemList();
    }
    return nullptr;
}

// BLOCK_ITEM -> DECLARATION | STATEMENT
std::unique_ptr<BlockItemBase> Parser::parseBlockItem()
{
    if (isBasicType())
    {
        std::unique_ptr<Declaration> declaration = parseDeclaration();
        return std::make_unique<DeclarationWrapper>(std::move(declaration), currentToken.getLineNumber());
    }
    else if (isStatementFirst())
    {
        std::unique_ptr<Stmt> statement = parseStatement();
        return std::make_unique<StatementWrapper>(std::move(statement), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected declaration or statement");
    }
    return nullptr;
}

// BLOCK_ITEM_LIST -> BLOCK_ITEM BLOCK_ITEM_LIST_PRIME
std::unique_ptr<BlockStmt> Parser::parseBlockItemList()
{
    std::vector<std::unique_ptr<BlockItemBase>> blockItems;
    blockItems.push_back(parseBlockItem());
    // BLOCK_ITEM_LIST_PRIME -> BLOCK_ITEM BLOCK_ITEM_LIST_PRIME | ϵ
    while (isBlockFirst())
    {
        blockItems.push_back(parseBlockItem());
    }
    return std::make_unique<BlockStmt>(std::move(blockItems), currentToken.getLineNumber());
}

// CONDITIONAL_EXPRESSION -> LOGICAL_OR_EXPRESSION CONDITIONAL_EXPRESSION_PRIME
std::unique_ptr<ConditionalExpression> Parser::parseConditionalExpression()
{
    std::unique_ptr<Expr> logicalOrExpr = parseLogicalOrExpression();

    // CONDITIONAL_EXPRESSION_PRIME -> ? EXPRESSION : CONDITIONAL_EXPRESSION | ϵ
    if (currentToken.getType() == TokenType::QUESTION_MARK)
    {
        consume();
        std::unique_ptr<Expr> expr = parseEXPRESSION();
        if (currentToken.getType() == TokenType::COLON)
        {
            consume();
        }
        else
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ':' Token");
        }
        std::unique_ptr<ConditionalExpression> conditionalExpr = parseConditionalExpression();
        return std::make_unique<ConditionalExpression>(std::move(logicalOrExpr), std::move(expr), std::move(conditionalExpr), currentToken.getLineNumber());
    }
    return std::make_unique<ConditionalExpression>(std::move(logicalOrExpr), nullptr, nullptr, currentToken.getLineNumber());
}

// CONSTANT -> INTEGER_CONSTANT | FLOAT_CONSTANT | STRING_LITERAL
std::unique_ptr<ConstantExpression> Parser::parseConstant()
{
    if (currentToken.getType() == TokenType::INTEGER_CONSTANT)
    {
        std::unique_ptr<ConstantExpression> constant = std::make_unique<ConstantExpression>(currentToken.getValue(), ConstantType::INTEGER_CONSTANT, currentToken.getLineNumber());
        consume();
        return constant;
    }
    else if (currentToken.getType() == TokenType::FLOAT_CONSTANT)
    {
        std::unique_ptr<ConstantExpression> constant = std::make_unique<ConstantExpression>(currentToken.getValue(), ConstantType::FLOAT_CONSTANT, currentToken.getLineNumber());
        consume();
        return constant;
    }
    else if (currentToken.getType() == TokenType::STRING_LITERAL)
    {
        std::unique_ptr<ConstantExpression> constant = std::make_unique<ConstantExpression>(currentToken.getValue(), ConstantType::STRING_LITERAL, currentToken.getLineNumber());
        consume();
        return constant;
    }
    return nullptr;
}

// DECLARATION_LIST -> DECLARATION DECLARATION_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseDeclarationList()
{
    parseDeclaration();
    parseDeclarationListPrime();
    return nullptr;
}

// DECLARATION_LIST_PRIME -> DECLARATION DECLARATION_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseDeclarationListPrime()
{
    if (isBasicType())
    {
        parseDeclaration();
        parseDeclarationListPrime();
    }
    return nullptr;
}

// DECLARATION_SPECIFIERS -> TYPE_SPECIFIER DECLARATION_SPECIFIERS_PRIME
std::unique_ptr<DeclarationSpecifiers> Parser::parseDeclarationSpecifiers()
{
    std::unique_ptr<Type> type = parseTypeSpecifier();
    std::unique_ptr<Specifier> specifier = parseDeclarationSpecifiersPrime();
    if (specifier != nullptr)
    {
        return std::make_unique<DeclarationSpecifiers>(std::move(type), std::move(specifier), currentToken.getLineNumber());
    }
    return std::make_unique<DeclarationSpecifiers>(std::move(type), nullptr, currentToken.getLineNumber());
}

// FIRST(TYPE_QUALIFIER)=  const restrict volatile
// FIRST(FUNCTION_SPECIFIER) = inline
// FIRST(STORAGE_SPECIFIER) =	auto register static

// DECLARATION_SPECIFIERS_PRIME -> TYPE_QUALIFIER | FUNCTION_SPECIFIER | STORAGE_SPECIFIER | ϵ
std::unique_ptr<Specifier> Parser::parseDeclarationSpecifiersPrime()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::CONST || type == TokenType::RESTRICT || type == TokenType::VOLATILE)
    {
        return parseTypeQualifier();
    }
    else if (type == TokenType::INLINE)
    {
        return parseFunctionSpecifier();
    }
    else if (type == TokenType::AUTO || type == TokenType::REGISTER || type == TokenType::STATIC)
    {
        return parseStorageSpecifier();
    }
    return nullptr;
}

// FIRST(DIRECT_DECLARATOR) = 	( IDENTIFIER
// FIRST(POINTER) = *
// DECLARATOR -> POINTER DIRECT_DECLARATOR | DIRECT_DECLARATOR
std::unique_ptr<Declarator> Parser::parseDeclarator()
{
    if (currentToken.getType() == TokenType::ASTERISK)
    {
        std::unique_ptr<Declarator> declarator = std::make_unique<Declarator>(currentToken.getLineNumber());
        declarator->setPointer(parsePointer());
        parseDirectDeclarator(declarator);
        return declarator;
    }
    else if (currentToken.getType() == TokenType::LPAREN || currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::unique_ptr<Declarator> declarator = std::make_unique<Declarator>(currentToken.getLineNumber());
        parseDirectDeclarator(declarator);
        return declarator;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected POINTER or DIRECT_DECLARATOR");
    }
}

// DIRECT_DECLARATOR -> IDENTIFIER DIRECT_DECLARATOR_PRIME | ( DECLARATOR ) DIRECT_DECLARATOR_PRIME
void Parser::parseDirectDeclarator(std::unique_ptr<Declarator> &declarator)
{
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        declarator->setIdentifier(currentToken.getValue());
        consume();
        parseDirectDeclaratorPrime(declarator);
    }
    // for function pointer declaration
    else if (currentToken.getType() == TokenType::LPAREN)
    {
        consume();

        std::unique_ptr<Declarator> functionDeclarator = parseDeclarator();
        if (functionDeclarator != nullptr)
        {
            declarator->setDeclarator(std::move(functionDeclarator));
        }

        if (currentToken.getType() != TokenType::RPAREN)
        {

            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in declarator, instead got: " + currentToken.getValue());
        }
        consume();
        parseDirectDeclaratorPrime(declarator);
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected IDENTIFIER or ( in declarator");
    }
}

// DIRECT_DECLARATOR_PRIME -> [ STATIC_OPT_FAC TYPE_QUALIFIER_LIST_OPT_FAC	ASSIGNMENT_EXPRESSION_OPT_FAC ] DIRECT_DECLARATOR_PRIME | ( PARAMETER_LIST_OPT_FAC ) DIRECT_DECLARATOR_PRIME | ϵ
void Parser::parseDirectDeclaratorPrime(std::unique_ptr<Declarator> &declarator)
{
    TokenType type = currentToken.getType();
    while (type == TokenType::LBRACKET || type == TokenType::LPAREN)
    {
        // in array definition
        if (currentToken.getType() == TokenType::LBRACKET)
        {
            consume();
            bool isStatic = parseStaticOptFac();
            std::unique_ptr<Specifier> typeQualifier = parseTypeQualifierListOptFac();
            std::unique_ptr<Expr> expr = parseAssignmentExpressionOptFac();
            if (currentToken.getType() != TokenType::RBRACKET)
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected closing ] in declarator");
            }
            consume();
            std::unique_ptr<ArrayDirectDeclarator> arrayDeclarator = std::make_unique<ArrayDirectDeclarator>(std::move(expr), std::move(typeQualifier), isStatic, currentToken.getLineNumber());
            declarator->addDirectDeclarator(std::move(arrayDeclarator));
        }
        // in function definition
        else if (currentToken.getType() == TokenType::LPAREN)
        {
            consume();
            std::unique_ptr<FunctionDirectDeclarator> funcDeclarator = parseParameterListOptFac();
            if (currentToken.getType() != TokenType::RPAREN)
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in declarator, instead got: " + currentToken.getValue());
            }
            consume();
            declarator->addDirectDeclarator(std::move(funcDeclarator));
        }
        type = currentToken.getType();
    }
}

// EQUALITY_EXPRESSION -> RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseEqualityExpression()
{
    std::unique_ptr<Expr> lhs = parseRelationalExpression();

    // EQUALITY_EXPRESSION_PRIME -> == RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME | != RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::EQ_OP || currentToken.getType() == TokenType::NE_OP)
    {
        if (currentToken.getType() == TokenType::EQ_OP)
        {
            consume();
            std::unique_ptr<Expr> rhs = parseRelationalExpression();
            lhs = std::make_unique<BinaryExpr>(std::move(lhs), "==", std::move(rhs), currentToken.getLineNumber());
        }
        else if (currentToken.getType() == TokenType::NE_OP)
        {
            consume();
            std::unique_ptr<Expr> rhs = parseRelationalExpression();
            lhs = std::make_unique<BinaryExpr>(std::move(lhs), "!=", std::move(rhs), currentToken.getLineNumber());
        }
    }
    return lhs;
}

// EXPRESSION -> ASSIGNMENT_EXPRESSION EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseEXPRESSION()
{
    std::vector<std::unique_ptr<AssignmentExpression>> expressions;
    expressions.push_back(parseAssignmentExpression());

    // EXPRESSION_PRIME -> , ASSIGNMENT_EXPRESSION EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMMA)
    {
        consume();
        expressions.push_back(parseAssignmentExpression());
    }
    return std::make_unique<AssignmentExpressionList>(std::move(expressions), currentToken.getLineNumber());
}

// EXPRESSION_STATEMENT -> ; | EXPRESSION ;
std::unique_ptr<Expr> Parser::parseExpressionStatement()
{
    if (currentToken.getType() == TokenType::SEMICOLON)
    {
        consume();
        // empty statement
    }
    else if (isExpressionFirst())
    {
        std::unique_ptr<Expr> expression = parseEXPRESSION();
        if (currentToken.getType() != TokenType::SEMICOLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ; at the end of an expression");
        }
        consume();
        return expression;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected closing semicolon ; or expression.");
    }

    return nullptr;
}

// FIRST(EXPRESSION_STATEMENT)=    ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// FOR_INIT_STATEMENT -> EXPRESSION_STATEMENT EXPRESSION_STATEMENT FOR_OPTIONAL_EXPRESSION | DECLARATION EXPRESSION_STATEMENT FOR_OPTIONAL_EXPRESSION
std::unique_ptr<ForInitStatement> Parser::parseForInitStatement()
{
    if (isDeclarationFirst())
    {
        std::unique_ptr<Declaration> declaration = parseDeclaration();
        std::unique_ptr<Expr> expression = parseExpressionStatement();
        std::unique_ptr<Expr> optionalExpression = parseForOptionalExpression();
        return std::make_unique<ForWitDeclaration>(std::move(declaration), std::move(expression), std::move(optionalExpression), currentToken.getLineNumber());
    }
    else if (isExpressionFirst() || currentToken.getType() == TokenType::SEMICOLON)
    {
        return std::make_unique<ForWithExpression>(parseExpressionStatement(), parseExpressionStatement(), parseForOptionalExpression(), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected EXPRESSION_STATEMENT or DECLARATION.");
    }
    return nullptr;
}

// FOR_OPTIONAL_EXPRESSION -> EXPRESSION | ϵ
std::unique_ptr<Expr> Parser::parseForOptionalExpression()
{
    if (isExpressionFirst())
    {
        return parseEXPRESSION();
    }
    return nullptr;
}

// FUNCTION_DEFINITION -> func DECLARATION_SPECIFIERS DECLARATOR FUNCTION_DEF_FAC
std::unique_ptr<FunctionDecl> Parser::parseFunctionDefinition()
{
    if (currentToken.getType() == TokenType::FUNC)
    {
        consume();
        std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers = parseDeclarationSpecifiers();
        std::unique_ptr<Declarator> declarator = parseDeclarator();
        std::unique_ptr<BlockStmt> block = parseFunctionDefFac();
        std::unique_ptr<FunctionDecl> functionDecl = std::make_unique<FunctionDecl>(
            std::move(declarationSpecifiers),
            std::move(declarator),
            std::move(block), 
            currentToken.getLineNumber());
        return functionDecl;
    }

    return nullptr;
}

// FIRST(DECLARATION_LIST)=	    bool char double float int long short signed struct unsigned void
// FIRST(BLOCK) = {
//  FUNCTION_DEF_FAC -> DECLARATION_LIST BLOCK | BLOCK
std::unique_ptr<BlockStmt> Parser::parseFunctionDefFac()
{
    if (isBasicType())
    {
        parseDeclarationList();
        return parseBlock();
    }
    else if (currentToken.getType() == TokenType::LBRACE)
    {
        return parseBlock();
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected declarations or block {}");
    }

    return nullptr;
}

// FUNCTION_SPECIFIER -> inline
std::unique_ptr<Specifier> Parser::parseFunctionSpecifier()
{
    if (currentToken.getType() == TokenType::INLINE)
    {
        // save it here
        consume();
        return std::make_unique<Specifier>(currentToken.getValue(), "function", currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected function specifier Token");
    }

    return nullptr;
}

// IDENTIFIER_LIST -> IDENTIFIER IDENTIFIER_LIST_PRIME
std::unique_ptr<IdentifierList> Parser::parseIdentifierList()
{
    std::vector<std::string> identifiers;
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        identifiers.push_back(currentToken.getValue());
        consume();
        // IDENTIFIER_LIST_PRIME -> , IDENTIFIER IDENTIFIER_LIST_PRIME | ϵ
        while (currentToken.getType() == TokenType::COMMA)
        {
            consume();
            if (currentToken.getType() == TokenType::IDENTIFIER)
            {
                identifiers.push_back(currentToken.getValue());
                consume();
            }
            else
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected identifier");
            }
        }
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected identifier");
    }
    if (identifiers.size() == 0)
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected identifier");
    }
    return std::make_unique<IdentifierList>(std::move(identifiers), currentToken.getLineNumber());
}

// FIRST(INITIALIZER_BRACE_FAC)=	{
// FIRST(ASSIGMENT_EXPRESSION)=	( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// INITIALIZER -> INITIALIZER_BRACE_FAC | ASSIGNMENT_EXPRESSION
std::unique_ptr<Initializer> Parser::parseInitializer()
{
    if (currentToken.getType() == TokenType::LBRACE)
    {
        std::unique_ptr<BraceInitializer> braceInitializer = std::make_unique<BraceInitializer>(std::move(parseInitializerBraceFac()), currentToken.getLineNumber());
        return braceInitializer;
    }
    else if (isExpressionFirst())
    {
        std::unique_ptr<ExpressionInitializer> ExpInitializer = std::make_unique<ExpressionInitializer>(std::move(parseAssignmentExpression()), currentToken.getLineNumber());
        return ExpInitializer;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected initializer");
    }
    return nullptr;
}

// INITIALIZER_BRACE_FAC -> { INITIALIZER_LIST }
std::unique_ptr<InitializerList> Parser::parseInitializerBraceFac()
{
    if (currentToken.getType() == TokenType::LBRACE)
    {
        consume();
        std::unique_ptr<InitializerList> initializer = parseInitializerList();
        if (currentToken.getType() == TokenType::RBRACE)
        {
            consume();
        }
        else
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing } in initializer");
        }
        return initializer;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected opening { in initializer");
    }

    return nullptr;
}

// INITIALIZER_LIST -> INITIALIZER INITIALIZER_LIST_PRIME
std::unique_ptr<InitializerList> Parser::parseInitializerList()
{
    std::vector<std::unique_ptr<Initializer>> initializers;
    initializers.push_back(parseInitializer());
    // INITIALIZER_LIST_PRIME -> , INITIALIZER INITIALIZER_LIST_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMMA)
    {
        consume();
        initializers.push_back(parseInitializer());
    }
    return std::make_unique<InitializerList>(std::move(initializers), currentToken.getLineNumber());

    return nullptr;
}

// INIT_DECLARATOR -> DECLARATOR INIT_DECLARATOR_FAC
std::unique_ptr<Declarator> Parser::parseInitDeclarator()
{
    std::unique_ptr<Declarator> declarator = parseDeclarator();
    if (declarator == nullptr)
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected declarator");
    }
    declarator->setInitializer(parseInitDeclaratorFac());
    return declarator;
}

// INIT_DECLARATOR_FAC -> = INITIALIZER | ϵ
std::unique_ptr<Initializer> Parser::parseInitDeclaratorFac()
{
    if (currentToken.getType() == TokenType::EQUAL)
    {
        consume();
        return parseInitializer();
    }

    return nullptr;
}

// INIT_DECLARATOR_LIST -> INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME
std::unique_ptr<DeclaratorList> Parser::parseInitDeclaratorList()
{
    std::vector<std::unique_ptr<Declarator>> declarators;
    declarators.push_back(std::move(parseInitDeclarator()));

    // INIT_DECLARATOR_LIST_PRIME -> , INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMMA)
    {
        consume();
        declarators.push_back(std::move(parseInitDeclarator()));
    }
    return std::make_unique<DeclaratorList>(std::move(declarators), currentToken.getLineNumber());
}

// ITERATION_STATEMENT -> while ( EXPRESSION ) STATEMENT | do STATEMENT while ( EXPRESSION ) ; | for ( FOR_INIT_STATEMENT ) STATEMENT
std::unique_ptr<IterationStatement> Parser::parseIterationStatement()
{
    if (currentToken.getType() == TokenType::WHILE)
    {
        consume();
        if (currentToken.getType() != TokenType::LPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected opening ( in while statement");
        }

        consume();
        std::unique_ptr<Expr> expr = parseEXPRESSION();
        if (currentToken.getType() != TokenType::RPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in while statement");
        }
        consume();
        return std::make_unique<WhileStatement>(std::move(expr), parseStatement(), currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::DO)
    {
        consume();
        std::unique_ptr<Stmt> statement = parseStatement();
        if (currentToken.getType() != TokenType::WHILE)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected while in do-while statement");
        }
        consume();
        if (currentToken.getType() != TokenType::LPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected opening ( in do-while statement");
        }
        consume();
        std::unique_ptr<Expr> expr = parseEXPRESSION();
        if (currentToken.getType() != TokenType::RPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in do-while statement");
        }
        consume();
        if (currentToken.getType() != TokenType::SEMICOLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ; in do-while statement");
        }
        consume();
        return std::make_unique<DoWhileStatement>(std::move(expr), std::move(statement), currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::FOR)
    {
        consume();
        if (currentToken.getType() != TokenType::LPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected opening ( in for statement");
        }
        consume();
        std::unique_ptr<ForInitStatement> forInitStatement = parseForInitStatement();
        if (currentToken.getType() != TokenType::RPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in for statement");
        }
        consume();
        return std::make_unique<ForStatement>(std::move(forInitStatement), parseStatement(), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected while, do or for statement");
    }

    return nullptr;
}

// JUMP_STATEMENT -> continue ; | break ; | return EXPRESSION_STATEMENT
std::unique_ptr<JumpStatement> Parser::parseJumpStatement()
{
    if (currentToken.getType() == TokenType::CONTINUE)
    {
        consume();
        if (currentToken.getType() != TokenType::SEMICOLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ; in continue statement");
        }
        consume();
        return std::make_unique<ContinueStatement>(currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::BREAK)
    {
        consume();
        if (currentToken.getType() != TokenType::SEMICOLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ; in break statement");
        }
        consume();
        return std::make_unique<BreakStatement>(currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::RETURN)
    {
        consume();
        return std::make_unique<ReturnStatement>(std::move(parseExpressionStatement()), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected continue, break or return statement");
    }

    return nullptr;
}

// LABELED_STATEMENT -> case CONDITIONAL_EXPRESSION : STATEMENT | default : STATEMENT
std::unique_ptr<LabaledStatement> Parser::parseLabeledStatement()
{
    if (currentToken.getType() == TokenType::CASE)
    {
        consume();
        std::unique_ptr<Expr> expr = parseConditionalExpression();
        if (currentToken.getType() != TokenType::COLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected : in case statement");
        }
        consume();
        std::unique_ptr<Stmt> statement = parseStatement();
        return std::make_unique<CaseStatement>(std::move(expr), std::move(statement), currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::DEFAULT)
    {
        consume();
        if (currentToken.getType() != TokenType::COLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected : in default statement");
        }
        consume();
        std::unique_ptr<Stmt> statement = parseStatement();
        return std::make_unique<DefaultStatement>(std::move(statement), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected case or default statement");
    }

    return nullptr;
}

// EXCLUSIVE_OR_EXPRESSION -> AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseExclusiveOrExpression()
{
    std::unique_ptr<Expr> lhs = parseAndExpression();

    // EXCLUSIVE_OR_EXPRESSION_PRIME -> ^ AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::CARET)
    {
        consume();
        std::unique_ptr<Expr> rhs = parseAndExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "^", std::move(rhs), currentToken.getLineNumber());
    }
    return lhs;
}

// INCLUSIVE_OR_EXPRESSION -> EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseInclusiveOrExpression()
{
    std::unique_ptr<Expr> lhs = parseExclusiveOrExpression();
    while (currentToken.getType() == TokenType::PIPE)
    {
        consume();
        std::unique_ptr<Expr> rhs = parseExclusiveOrExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "|", std::move(rhs), currentToken.getLineNumber());
    }
    return lhs;
}

// LOGICAL_AND_EXPRESSION -> INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseLogicalAndExpression()
{
    std::unique_ptr<Expr> lhs = parseInclusiveOrExpression();

    // LOGICAL_AND_EXPRESSION_PRIME -> && INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::AND_OP)
    {
        consume();
        std::unique_ptr<Expr> rhs = parseInclusiveOrExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "&&", std::move(rhs), currentToken.getLineNumber());
    }
    return lhs;
}

// LOGICAL_OR_EXPRESSION -> LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseLogicalOrExpression()
{
    std::unique_ptr<Expr> lhs = parseLogicalAndExpression();

    // LOGICAL_OR_EXPRESSION_PRIME -> || LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::OR_OP)
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseLogicalAndExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs), currentToken.getLineNumber());
    }

    return lhs;
}

// ADDITIVE_EXPRESSION -> MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseAdditveExpression()
{
    //  ( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
    std::unique_ptr<Expr> lhs = parseMultiplicativeExpression();

    // ADDITIVE_EXPRESSION_PRIME -> + MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME | - MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::PLUS || currentToken.getType() == TokenType::MINUS)
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseMultiplicativeExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs), currentToken.getLineNumber());
    }
    return lhs;
}

// AND_EXPRESSION -> EQUALITY_EXPRESSION AND_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseAndExpression()
{
    std::unique_ptr<Expr> lhs = parseEqualityExpression();

    // AND_EXPRESSION_PRIME -> & EQUALITY_EXPRESSION AND_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::AMPERSAND)
    {
        consume();
        std::unique_ptr<Expr> rhs = parseEqualityExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "&", std::move(rhs), currentToken.getLineNumber());
    }
    return lhs;
}

// MULTIPLICATIVE_EXPRESSION -> UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseMultiplicativeExpression()
{
    std::unique_ptr<Expr> lhs = parseUnaryExpression();

    TokenType type = currentToken.getType();

    // MULTIPLICATIVE_EXPRESSION_PRIME -> * UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | / UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | % UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | ϵ
    while (type == TokenType::ASTERISK || type == TokenType::SLASH || type == TokenType::PERCENT)
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseUnaryExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs), currentToken.getLineNumber());
        type = currentToken.getType();
    }
    return lhs;
}

// FIRST(CONSTANT)=  INTEGER_CONSTANT FLOAT_CONSTANT STRING_LITERAL
//  PRIMARY_EXPRESSION -> IDENTIFIER | CONSTANT | ( EXPRESSION )
std::unique_ptr<PrimaryExpression> Parser::parsePrimaryExpression()
{
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::string identifier = currentToken.getValue();
        consume();
        return std::make_unique<IdentifierExpression>(identifier, currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::LPAREN)
    {
        consume();
        std::unique_ptr<ParenthesizedExpression> parenthesizedExpr = std::make_unique<ParenthesizedExpression>(std::move(parseEXPRESSION()), currentToken.getLineNumber());
        if (currentToken.getType() != TokenType::RPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in primary expression");
        }
        consume();
        return parenthesizedExpr;
    }
    else if (currentToken.getType() == TokenType::INTEGER_CONSTANT || currentToken.getType() == TokenType::FLOAT_CONSTANT || currentToken.getType() == TokenType::STRING_LITERAL)
    {
        return parseConstant();
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected identifier, constant or expression in primary expression");
    }

    return nullptr;
}

// RELATIONAL_EXPRESSION -> SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseRelationalExpression()
{
    std::unique_ptr<Expr> lhs = parseShiftExpression();

    TokenType type = currentToken.getType();

    // RELATIONAL_EXPRESSION_PRIME -> < SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | > SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | <= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | >= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | ϵ
    while (type == TokenType::LESS_THAN || type == TokenType::GREATER_THAN || type == TokenType::GE_OP || type == TokenType::LE_OP)

    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseShiftExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs), currentToken.getLineNumber());
        type = currentToken.getType();
    }
    return lhs;
}

// PARAMETER_DECLARATION -> DECLARATION_SPECIFIERS PARAM_DECL_FAC
std::unique_ptr<ParameterDeclaration> Parser::parseParameterDeclaration()
{
    std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers = parseDeclarationSpecifiers();
    std::unique_ptr<Declarator> declarator = parseParamDeclFac();
    return std::make_unique<ParameterDeclaration>(std::move(declarationSpecifiers), std::move(declarator), currentToken.getLineNumber());
}

// PARAMETER_LIST -> PARAMETER_DECLARATION PARAMETER_LIST_PRIME
std::unique_ptr<ParameterDeclarationList> Parser::parseParameterList()
{
    std::vector<std::unique_ptr<ParameterDeclaration>> parameters;
    parameters.push_back(parseParameterDeclaration());

    // PARAMETER_LIST_PRIME -> , PARAMETER_DECLARATION PARAMETER_LIST_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMMA)
    {
        consume();
        parameters.push_back(parseParameterDeclaration());
    }
    return std::make_unique<ParameterDeclarationList>(std::move(parameters), currentToken.getLineNumber());
}

// FIRST(IDENTIFIER_LIST)=  IDENTIFIER
//  FIRST(PARAMETER_LIST)=  bool char double float int long short signed struct unsigned void
//  PARAMETER_LIST_OPT_FAC -> PARAMETER_LIST | IDENTIFIER_LIST | ϵ
std::unique_ptr<FunctionDirectDeclarator> Parser::parseParameterListOptFac()
{
    if (isBasicType())
    {
        return parseParameterList();
    }
    else if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        return parseIdentifierList();
    }
    return nullptr;
}

// FIRST(DECLARATOR)=     ( * IDENTIFIER
// PARAM_DECL_FAC -> DECLARATOR | ϵ
std::unique_ptr<Declarator> Parser::parseParamDeclFac()
{
    if (currentToken.getType() == TokenType::LPAREN || currentToken.getType() == TokenType::ASTERISK || currentToken.getType() == TokenType::IDENTIFIER)
    {
        return parseDeclarator();
    }
    return nullptr;
}

// POINTER -> *
bool Parser::parsePointer()
{
    if (currentToken.getType() == TokenType::ASTERISK)
    {
        consume();
        return true;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected * in pointer");
    }
    return false;
}

// FIRST(ARGUMENT_EXPRESSION_LIST) = 	( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// POSTFIX_ARGUMENTS -> ARGUMENT_EXPRESSION_LIST | ϵ
std::unique_ptr<AssignmentExpressionList> Parser::parsePostfixArguments()
{
    if (isExpressionFirst())
    {
        return parseArgumentExpressionList();
    }

    return nullptr;
}

// POSTFIX_EXPRESSION -> PRIMARY_EXPRESSION POSTFIX_EXPRESSION_PRIME
std::unique_ptr<PostfixExpressions> Parser::parsePostfixExpression()
{
    std::unique_ptr<PrimaryExpression> primaryExpr = parsePrimaryExpression();
    std::vector<std::unique_ptr<PostfixExpressionBase>> postfixExpressions;

    // POSTFIX_EXPRESSION_PRIME -> [ EXPRESSION ] POSTFIX_EXPRESSION_PRIME | ( POSTFIX_ARGUMENTS ) POSTFIX_EXPRESSION_PRIME | . IDENTIFIER POSTFIX_EXPRESSION_PRIME | -> IDENTIFIER POSTFIX_EXPRESSION_PRIME | ++ POSTFIX_EXPRESSION_PRIME | -- POSTFIX_EXPRESSION_PRIME | ϵ
    while (true)
    {
        if (currentToken.getType() == TokenType::LBRACKET && currentToken.getValue() == "[")
        {
            consume();
            std::unique_ptr<ArrayPostFixExpression> arrayPostFixExpr = std::make_unique<ArrayPostFixExpression>(parseEXPRESSION(), currentToken.getLineNumber());
            if (currentToken.getType() != TokenType::RBRACKET || currentToken.getValue() != "]")
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected closing ] in postfix expression");
            }
            consume();
            postfixExpressions.push_back(std::move(arrayPostFixExpr));
        }
        else if (currentToken.getType() == TokenType::LPAREN)
        {
            consume();
            std::unique_ptr<ArgumentsPostFixExpression> argsPostFixExpr = std::make_unique<ArgumentsPostFixExpression>(parsePostfixArguments(), currentToken.getLineNumber());
            if (currentToken.getType() != TokenType::RPAREN)
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in postfix expression");
            }
            consume();
            postfixExpressions.push_back(std::move(argsPostFixExpr));
        }
        else if (currentToken.getType() == TokenType::DOT)
        {
            consume();
            if (currentToken.getType() != TokenType::IDENTIFIER)
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected identifier after . in postfix expression");
            }
            std::unique_ptr<DotOperatorPostfixExpression> dotOperatorPostfixExpr = std::make_unique<DotOperatorPostfixExpression>(currentToken.getValue(), currentToken.getLineNumber());
            consume();
            postfixExpressions.push_back(std::move(dotOperatorPostfixExpr));
        }
        else if (currentToken.getType() == TokenType::PTR_OP)
        {
            consume();
            if (currentToken.getType() != TokenType::IDENTIFIER)
            {
                throw SyntacticException(currentToken.getLineNumber(), "Expected identifier after -> in postfix expression");
            }
            std::unique_ptr<ArrowOperatorPostfixExpression> arrowOperatorPostfixExpr = std::make_unique<ArrowOperatorPostfixExpression>(currentToken.getValue(), currentToken.getLineNumber());
            consume();
            postfixExpressions.push_back(std::move(arrowOperatorPostfixExpr));
        }
        else if (currentToken.getType() == TokenType::INC_OP || currentToken.getType() == TokenType::DEC_OP)
        {
            std::unique_ptr<IncrementDecrementPostfixExpression> incrementOperatorPostfixExpr = std::make_unique<IncrementDecrementPostfixExpression>(currentToken.getValue(), currentToken.getLineNumber());
            consume();
            postfixExpressions.push_back(std::move(incrementOperatorPostfixExpr));
        }
        else
        {
            break;
        }
    }
    return std::make_unique<PostfixExpressions>(std::move(primaryExpr), std::move(postfixExpressions), currentToken.getLineNumber());
}

// SELECTION_STATEMENT -> if ( EXPRESSION ) STATEMENT | switch ( EXPRESSION ) STATEMENT
std::unique_ptr<SelectionStatement> Parser::parseSelectionStatement()
{
    if (currentToken.getType() == TokenType::IF)
    {
        consume();
        if (currentToken.getType() != TokenType::LPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected opening ( in if statement");
        }
        consume();
        std::unique_ptr<Expr> expression = parseEXPRESSION();
        if (currentToken.getType() != TokenType::RPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in if statement");
        }
        consume();
        return std::make_unique<IfStatement>(std::move(expression), std::move(parseStatement()), currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::SWITCH)
    {
        consume();
        if (currentToken.getType() != TokenType::LPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected opening ( in switch statement");
        }
        consume();
        std::unique_ptr<Expr> expression = parseEXPRESSION();
        if (currentToken.getType() != TokenType::RPAREN)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing ) in switch statement");
        }
        consume();
        return std::make_unique<SwitchStatement>(std::move(expression), std::move(parseStatement()), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected if or switch statement");
    }

    return nullptr;
}

// SHIFT_EXPRESSION -> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseShiftExpression()
{
    std::unique_ptr<Expr> lhs = parseAdditveExpression();

    // SHIFT_EXPRESSION_PRIME -> << ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | >> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::RIGHT_OP || currentToken.getType() == TokenType::LEFT_OP)
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseAdditveExpression();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs), currentToken.getLineNumber());
    }
    return lhs;
}

// FIRST(LABALED_STATEMENT)=  case default
// FIRST(BLOCK)= {
// FIRST(EXPRESSION_STATEMENT)=  ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// FIRST(SELECTION_STATEMENT)=  if switch
// FIRST(ITERATION_STATEMENT)=  while do for
// FIRST(JUMP_STATEMENT)=  continue break return
//  STATEMENT -> LABELED_STATEMENT | BLOCK | EXPRESSION_STATEMENT | SELECTION_STATEMENT | ITERATION_STATEMENT | JUMP_STATEMENT
std::unique_ptr<Stmt> Parser::parseStatement()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::CASE || type == TokenType::DEFAULT)
    {
        return parseLabeledStatement();
    }
    else if (type == TokenType::LBRACE)
    {
        return parseBlock();
    }
    else if (isExpressionFirst() || type == TokenType::SEMICOLON)
    {
        std::unique_ptr<Expr> expression = parseExpressionStatement();
        return std::make_unique<ExpressionStatement>(std::move(expression), currentToken.getLineNumber());
    }
    else if (type == TokenType::IF || type == TokenType::SWITCH)
    {
        return parseSelectionStatement();
    }
    else if (type == TokenType::WHILE || type == TokenType::DO || type == TokenType::FOR)
    {
        return parseIterationStatement();
    }
    else if (type == TokenType::CONTINUE || type == TokenType::BREAK || type == TokenType::RETURN)
    {
        return parseJumpStatement();
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected statement");
    }

    return nullptr;
}

// STATIC_OPT_FAC -> static | ϵ
bool Parser::parseStaticOptFac()
{
    if (currentToken.getType() == TokenType::STATIC)
    {
        true;
        consume();
    }

    return false;
}

// STORAGE_SPECIFIER -> static | auto | register
std::unique_ptr<Specifier> Parser::parseStorageSpecifier()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::STATIC || type == TokenType::AUTO || type == TokenType::REGISTER)
    {
        consume();
        return std::make_unique<Specifier>(currentToken.getValue(), "Storage", currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected storage specifier Token");
    }

    return nullptr;
}

// STRUCT_DECLARATION -> SPECIFIER_QUALIFIER_LIST STRUCT_DECL_FAC
std::unique_ptr<StructMemberDeclaration> Parser::parseSTRUCT_DECLARATION()
{
    // SPECIFIER_QUALIFIER_LIST -> TYPE_SPECIFIER
    std::unique_ptr<Type> Type = parseTypeSpecifier();
    std::unique_ptr<DeclaratorList> declaratorList = parseSTRUCT_DECL_FAC();
    return std::make_unique<StructMemberDeclaration>(std::move(Type), std::move(declaratorList), currentToken.getLineNumber());
}

// STRUCT_DECLARATION_LIST -> STRUCT_DECLARATION STRUCT_DECLARATION_LIST_PRIME
std::unique_ptr<StructMemberDeclarationList> Parser::parseSTRUCT_DECLARATION_LIST()
{
    std::vector<std::unique_ptr<StructMemberDeclaration>> structMemberDeclarations;
    structMemberDeclarations.push_back(parseSTRUCT_DECLARATION());

    // FIRST(STRUCT_DECLARATION) = bool char double float int long short signed struct unsigned void
    // STRUCT_DECLARATION_LIST_PRIME -> STRUCT_DECLARATION STRUCT_DECLARATION_LIST_PRIME | ϵ
    while (isBasicType())
    {
        structMemberDeclarations.push_back(parseSTRUCT_DECLARATION());
    }
    return std::make_unique<StructMemberDeclarationList>(std::move(structMemberDeclarations), currentToken.getLineNumber());
}

// STRUCT_DECLARATOR_LIST -> DECLARATOR STRUCT_DECLARATOR_LIST_PRIME
std::unique_ptr<DeclaratorList> Parser::parseSTRUCT_DECLARATOR_LIST()
{
    std::vector<std::unique_ptr<Declarator>> declarators;
    declarators.push_back(parseDeclarator());

    // STRUCT_DECLARATOR_LIST_PRIME -> , DECLARATOR STRUCT_DECLARATOR_LIST_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMMA)
    {
        consume();
        declarators.push_back(parseDeclarator());
    }
    return std::make_unique<DeclaratorList>(std::move(declarators), currentToken.getLineNumber());
}

// FIRST(STRUCT_DECLARATOR_LIST)=    ( * IDENTIFIER
//  STRUCT_DECL_FAC -> ; | STRUCT_DECLARATOR_LIST ;
std::unique_ptr<DeclaratorList> Parser::parseSTRUCT_DECL_FAC()
{
    if (currentToken.getType() == TokenType::SEMICOLON)
    {
        consume(); // empty declaration
    }
    else if (currentToken.getType() == TokenType::LPAREN || currentToken.getType() == TokenType::ASTERISK || currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::unique_ptr<DeclaratorList> declaratorList = parseSTRUCT_DECLARATOR_LIST();
        if (currentToken.getType() != TokenType::SEMICOLON)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected ; in struct declaration");
        }
        consume();
        return declaratorList;
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected ; or struct declarator list");
    }

    return nullptr;
}

// STRUCT_SPECIFIER -> struct STRUCT_SPEC_FAC
std::unique_ptr<StructSpecifier> Parser::parseSTRUCT_SPECIFIER()
{
    if (currentToken.getType() == TokenType::STRUCT)
    {
        consume();
        return parseSTRUCT_SPEC_FAC();
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected 'struct' specifier Token");
    }
    return nullptr;
}

// STRUCT_SPEC_FAC -> { STRUCT_DECLARATION_LIST } | IDENTIFIER STRUCT_SPEC_FAC2
std::unique_ptr<StructSpecifier> Parser::parseSTRUCT_SPEC_FAC()
{
    if (currentToken.getType() == TokenType::LBRACE)
    { // in anonimous struct
        consume();
        std::unique_ptr<StructMemberDeclarationList> structMemberDeclarationList = parseSTRUCT_DECLARATION_LIST();
        if (currentToken.getType() != TokenType::RBRACE)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing } in struct specifier");
        }
        consume();
        return std::make_unique<AnonimousStruct>(std::move(structMemberDeclarationList), currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::string identifier = currentToken.getValue();
        consume();
        std::unique_ptr<StructMemberDeclarationList> structMemberDeclarationList = parseSTRUCT_SPEC_FAC2();
        return std::make_unique<StructDeclaration>(std::move(structMemberDeclarationList), identifier, currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected { or IDENTIFIER in struct specifier");
    }
    return nullptr;
}

// STRUCT_SPEC_FAC2 -> { STRUCT_DECLARATION_LIST } | ϵ
std::unique_ptr<StructMemberDeclarationList> Parser::parseSTRUCT_SPEC_FAC2()
{
    if (currentToken.getType() == TokenType::LBRACE)
    {
        consume();
        std::unique_ptr<StructMemberDeclarationList> result = parseSTRUCT_DECLARATION_LIST();
        if (currentToken.getType() != TokenType::RBRACE)
        {
            throw SyntacticException(currentToken.getLineNumber(), "Expected closing } in struct specifier");
        }
        consume();
        return result;
    }
    return nullptr;
}

// TYPE_QUALIFIER -> const | restrict | volatile
std::unique_ptr<Specifier> Parser::parseTypeQualifier()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::CONST || type == TokenType::RESTRICT || type == TokenType::VOLATILE)
    {
        std::string specifier = currentToken.getValue();
        std::string type = "Qualifier";
        consume();
        return std::make_unique<Specifier>(specifier, type, currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected type qualifier Token");
    }

    return nullptr;
}

// TYPE_QUALIFIER_LIST_OPT_FAC -> TYPE_QUALIFIER | ϵ
std::unique_ptr<Specifier> Parser::parseTypeQualifierListOptFac()
{
    TokenType type = currentToken.getType();

    if (type == TokenType::CONST || type == TokenType::RESTRICT || type == TokenType::VOLATILE)
    {
        return parseTypeQualifier();
    }
    // (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "const" || currentToken.getValue() == "restrict" || currentToken.getValue() == "volatile"))
    // {
    //     return parseTypeQualifier();
    // }
    return nullptr;
}

// TYPE_SPECIFIER -> int | void | char | short | long | float | double | signed | unsigned | bool | STRUCT_SPECIFIER
std::unique_ptr<Type> Parser::parseTypeSpecifier()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::INT || type == TokenType::VOID || type == TokenType::CHAR || type == TokenType::SHORT || type == TokenType::LONG || type == TokenType::FLOAT || type == TokenType::DOUBLE || type == TokenType::SIGNED || type == TokenType::UNSIGNED || type == TokenType::BOOL)

    // currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "int" || currentToken.getValue() == "void" || currentToken.getValue() == "char" || currentToken.getValue() == "short" || currentToken.getValue() == "long" || currentToken.getValue() == "float" || currentToken.getValue() == "double" || currentToken.getValue() == "signed" || currentToken.getValue() == "unsigned" || currentToken.getValue() == "bool"))
    {
        std::string type = currentToken.getValue();
        consume();
        return std::make_unique<PrimitiveType>(type, currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::STRUCT)
    {
        return parseSTRUCT_SPECIFIER();
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected type specifier Token");
    }

    return nullptr;
}

// FIRST(POSTFIX_EXPRESSION)=     ( FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL

// UNARY_EXPRESSION -> POSTFIX_EXPRESSION | ++ UNARY_EXPRESSION | -- UNARY_EXPRESSION | sizeof UNARY_EXPRESSION
std::unique_ptr<UnaryExpr> Parser::parseUnaryExpression()
{
    TokenType type = currentToken.getType();
    if (type == TokenType::LPAREN || type == TokenType::FLOAT_CONSTANT || type == TokenType::IDENTIFIER || type == TokenType::INTEGER_CONSTANT || type == TokenType::STRING_LITERAL)
    {
        return parsePostfixExpression();
    }
    else if (type == TokenType::INC_OP || type == TokenType::DEC_OP)
    {
        std::string op = currentToken.getValue();
        consume();
        return std::make_unique<UnaryIncrementDecrementOperator>(std::move(op), parseUnaryExpression(), currentToken.getLineNumber());
    }
    else if (currentToken.getType() == TokenType::SIZEOF)
    {
        consume();
        return std::make_unique<UnaryIncrementDecrementOperator>("sizeof", parseUnaryExpression(), currentToken.getLineNumber());
    }
    else
    {
        throw SyntacticException(currentToken.getLineNumber(), "Expected postfix expression, ++, -- or sizeof");
    }

    return nullptr;
}