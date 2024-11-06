#include <stdexcept>
#include "Parser.h"
#include <unordered_set>
#include "PrintVisitor.h"
/*
HELPER FUNCTIONS
*/

//  bool char double float int long short signed struct unsigned void
bool Parser::isBasicType() const
{
    const std::string &tokenValue = currentToken.getValue();
    return tokenValue == "bool" || tokenValue == "char" || tokenValue == "double" ||
           tokenValue == "float" || tokenValue == "int" || tokenValue == "long" ||
           tokenValue == "short" || tokenValue == "signed" || tokenValue == "struct" ||
           tokenValue == "unsigned" || tokenValue == "void";
}

// ( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
bool Parser::isExpressionFirst() const
{
    const std::string &tokenValue = currentToken.getValue();

    return ((currentToken.getType() == TokenType::PUNCTUATION) && tokenValue == "(") ||
           (currentToken.getType() == TokenType::FLOAT_CONSTANT) ||
           (currentToken.getType() == TokenType::INTEGER_CONSTANT) ||
           (currentToken.getType() == TokenType::IDENTIFIER) ||
           (currentToken.getType() == TokenType::STRING_LITERAL) ||
           (currentToken.getType() == TokenType::INCREMENT_OPERATOR);
}

//  func bool char double float int long short signed struct unsigned void
bool Parser::isDeclarationFirst() const
{
    if (currentToken.getType() != TokenType::KEYWORD)
    {
        return false;
    }

    const std::string &tokenValue = currentToken.getValue();

    return isBasicType() || tokenValue == "func";
}

//       ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL bool break case char continue default do double float for if int long return short signed sizeof struct switch unsigned void while {
bool Parser::isBlockFirst() const
{
    return isBasicType() || isStatementFirst();
}

//       ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL break case continue default do for if return sizeof switch while {
bool Parser::isStatementFirst() const
{
    const std::string &tokenValue = currentToken.getValue();

    return (currentToken.getType() == TokenType::INCREMENT_OPERATOR) ||
           (currentToken.getType() == TokenType::FLOAT_CONSTANT) ||
           (currentToken.getType() == TokenType::INTEGER_CONSTANT) ||
           (currentToken.getType() == TokenType::IDENTIFIER) ||
           (currentToken.getType() == TokenType::STRING_LITERAL) ||
           (currentToken.getType() == TokenType::PUNCTUATION && (tokenValue == "{" || tokenValue == ";" || tokenValue == "(")) ||
           (currentToken.getType() == TokenType::KEYWORD && (tokenValue == "break" || tokenValue == "case" ||
                                                             tokenValue == "continue" || tokenValue == "default" ||
                                                             tokenValue == "do" || tokenValue == "for" || tokenValue == "if" ||
                                                             tokenValue == "return" || tokenValue == "sizeof" ||
                                                             tokenValue == "switch" || tokenValue == "while"));
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
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
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

    topLevelDeclarations.push_back(parseGLOBAL_DECLARATIONS());

    // PROGRAM_PRIME -> GLOBAL_DECLARATIONS PROGRAM_PRIME | ϵ
    while (isDeclarationFirst())
    {
        topLevelDeclarations.push_back(parseGLOBAL_DECLARATIONS());
    }

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    std::unique_ptr<Program> program = std::make_unique<Program>(std::move(topLevelDeclarations));

    PrintVisitor printVisitor;
    program->accept(printVisitor);

    return program;
}

// GLOBAL_DECLARATIONS -> DECLARATION | FUNCTION_DEFINITION
std::unique_ptr<ASTNode> Parser::parseGLOBAL_DECLARATIONS()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "func")
    {
        std::unique_ptr<FunctionDecl> functionDecl = parseFUNCTION_DEFINITION();
        return functionDecl;
    }
    else if (isBasicType())
    {
        std::unique_ptr<Declaration> declaration = parseDECLARATION();
        return declaration;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected declaration or function definition");
    }
}

// DECLARATION -> DECLARATION_SPECIFIERS DECLARATION_FAC
std::unique_ptr<Declaration> Parser::parseDECLARATION()
{
    std::unique_ptr<DeclarationSpecifiers> DeclarationSpec = parseDECLARATION_SPECIFIERS();
    std::unique_ptr<DeclatatorList> declaratorList = parseDECLARATION_FAC();
    return std::make_unique<Declaration>(std::move(DeclarationSpec), std::move(declaratorList));
}

// FIRST(INIT_DECLARATOR_LIST)= 	( * IDENTIFIER
// DECLARATION_FAC -> ; | INIT_DECLARATOR_LIST ;
std::unique_ptr<DeclatatorList> Parser::parseDECLARATION_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        consume();
        // empty declaration, example: int;
        return nullptr;
    }
    else if (
        (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(") || (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*") || (currentToken.getType() == TokenType::IDENTIFIER))
    {
        std::unique_ptr<DeclatatorList> declaratorList = parseINIT_DECLARATOR_LIST();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; at the end of declaration");
        }
        consume();
        return declaratorList;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected ; or INIT_DECLARATOR_LIST");
    }
}

// ARGUMENT_EXPRESSION_LIST -> ASSIGNMENT_EXPRESSION ARGUMENT_EXPRESSION_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseARGUMENT_EXPRESSION_LIST()
{
    parseASSIGNMENT_EXPRESSION();
    // ARGUMENT_EXPRESSION_LIST_PRIME -> , ASSIGNMENT_EXPRESSION ARGUMENT_EXPRESSION_LIST_PRIME | ϵ

    while (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parseASSIGNMENT_EXPRESSION();
    }
    return nullptr;
}

// ASSIGNMENT_EXPRESSION -> CONDITIONAL_EXPRESSION ASSIGNMENT_EXPRESSION_FAC
std::unique_ptr<Expr> Parser::parseASSIGNMENT_EXPRESSION()
{
    parseCONDITIONAL_EXPRESSION();
    parseASSIGNMENT_EXPRESSION_FAC();
    return nullptr;
}

// ASSIGNMENT_EXPRESSION_FAC -> ASSIGNMENT_OPERATOR ASSIGNMENT_EXPRESSION | ϵ
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_EXPRESSION_FAC()
{
    if (currentToken.getType() == TokenType::ASSIGNMENT_OPERATOR)
    {
        parseASSIGNMENT_OPERATOR();
        parseASSIGNMENT_EXPRESSION();
    }
    return nullptr;
}

// ASSIGNMENT_EXPRESSION_OPT_FAC -> ASSIGNMENT_EXPRESSION | ϵ
// First(ASSIGNMENT_EXPRESSION_OPT_FAC) = {( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof ϵ}
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_EXPRESSION_OPT_FAC()
{
    if (isExpressionFirst())
    {
        parseASSIGNMENT_EXPRESSION();
    }
    return nullptr;
}

// ASSIGNMENT_OPERATOR -> = | *= | /= | %= | += | -= | <<= | >>= | &&= | ^= | |=
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_OPERATOR()
{
    if (currentToken.getType() == TokenType::ASSIGNMENT_OPERATOR)
    {
        consume();
        // return true
    }
    return nullptr;
}

// BLOCK -> { BLOCK_CONTENT }
std::unique_ptr<BlockStmt> Parser::parseBLOCK()
{

    if ((currentToken.getType() == TokenType::PUNCTUATION) && (currentToken.getValue() == "{"))
    {
        consume();
        std::unique_ptr<BlockStmt> block = parseBLOCK_CONTENT();
        if ((currentToken.getType() == TokenType::PUNCTUATION) && (currentToken.getValue() == "}"))
        {
            consume();
            return block;
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected '}' at the end of block");
        }
    }

    return nullptr;
}

// BLOCK_CONTENT -> BLOCK_ITEM_LIST | ϵ
std::unique_ptr<BlockStmt> Parser::parseBLOCK_CONTENT()
{
    if (isBlockFirst())
    {
        return parseBLOCK_ITEM_LIST();
    }
    return nullptr;
}

// BLOCK_ITEM -> DECLARATION | STATEMENT
std::unique_ptr<BlockItemBase> Parser::parseBLOCK_ITEM()
{
    if (isBasicType())
    {
        std::unique_ptr<Declaration> declaration = parseDECLARATION();
        return std::make_unique<DeclarationWrapper>(std::move(declaration));
    }
    else if (isStatementFirst())
    {
        std::unique_ptr<Stmt> statement = parseSTATEMENT();
        return std::make_unique<StatementWrapper>(std::move(statement));
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected declaration or statement");
    }
    return nullptr;
}

// BLOCK_ITEM_LIST -> BLOCK_ITEM BLOCK_ITEM_LIST_PRIME
std::unique_ptr<BlockStmt> Parser::parseBLOCK_ITEM_LIST()
{
    std::vector<std::unique_ptr<BlockItemBase>> blockItems;
    blockItems.push_back(parseBLOCK_ITEM());
    // BLOCK_ITEM_LIST_PRIME -> BLOCK_ITEM BLOCK_ITEM_LIST_PRIME | ϵ
    while (isBlockFirst())
    {
        blockItems.push_back(parseBLOCK_ITEM());
    }
    return std::make_unique<BlockStmt>(std::move(blockItems));
}

// CONDITIONAL_EXPRESSION -> LOGICAL_OR_EXPRESSION CONDITIONAL_EXPRESSION_PRIME
std::unique_ptr<ConditionalExpression> Parser::parseCONDITIONAL_EXPRESSION()
{
    std::unique_ptr<Expr> logicalOrExpr = parseLOGICAL_OR_EXPRESSION();

    // CONDITIONAL_EXPRESSION_PRIME -> ? EXPRESSION : CONDITIONAL_EXPRESSION | ϵ
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "?")
    {
        consume();
        std::unique_ptr<Expr> expr = parseEXPRESSION();
        if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ":")
        {
            consume();
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected ':' Token");
        }
        std::unique_ptr<ConditionalExpression> conditionalExpr = parseCONDITIONAL_EXPRESSION();
        return std::make_unique<ConditionalExpression>(std::move(logicalOrExpr), std::move(expr), std::move(conditionalExpr));
    }
    return std::make_unique<ConditionalExpression>(std::move(logicalOrExpr), nullptr, nullptr);

    return nullptr;
}

// CONDITIONAL_EXPRESSION_PRIME -> ? EXPRESSION : CONDITIONAL_EXPRESSION | ϵ
std::unique_ptr<ASTNode> Parser::parseCONDITIONAL_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "?")
    {
        consume();
        parseEXPRESSION();
        if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ":")
        {
            consume();
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected ':' Token");
        }
        parseCONDITIONAL_EXPRESSION();
    }
    return nullptr;
}

// CONSTANT -> INTEGER_CONSTANT | FLOAT_CONSTANT | STRING_LITERAL
std::unique_ptr<ConstantExpression> Parser::parseCONSTANT()
{
    if (currentToken.getType() == TokenType::INTEGER_CONSTANT)
    {
        std::unique_ptr<ConstantExpression> constant = std::make_unique<ConstantExpression>(currentToken.getValue(), ConstantType::INTEGER_CONSTANT);
        consume();
        return constant;
    }
    else if (currentToken.getType() == TokenType::FLOAT_CONSTANT)
    {
        std::unique_ptr<ConstantExpression> constant = std::make_unique<ConstantExpression>(currentToken.getValue(), ConstantType::FLOAT_CONSTANT);
        consume();
        return constant;
    }
    else if (currentToken.getType() == TokenType::STRING_LITERAL)
    {
        std::unique_ptr<ConstantExpression> constant = std::make_unique<ConstantExpression>(currentToken.getValue(), ConstantType::STRING_LITERAL);
        consume();
        return constant;
    }
    return nullptr;
}

// DECLARATION_LIST -> DECLARATION DECLARATION_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseDECLARATION_LIST()
{
    parseDECLARATION();
    parseDECLARATION_LIST_PRIME();
    return nullptr;
}

// DECLARATION_LIST_PRIME -> DECLARATION DECLARATION_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseDECLARATION_LIST_PRIME()
{
    if (isBasicType())
    {
        parseDECLARATION();
        parseDECLARATION_LIST_PRIME();
    }
    return nullptr;
}

// DECLARATION_SPECIFIERS -> TYPE_SPECIFIER DECLARATION_SPECIFIERS_PRIME
std::unique_ptr<DeclarationSpecifiers> Parser::parseDECLARATION_SPECIFIERS()
{
    std::unique_ptr<Type> type = parseTYPE_SPECIFIER();
    std::unique_ptr<Specifier> specifier = parseDECLARATION_SPECIFIERS_PRIME();
    if (specifier != nullptr)
    {
        return std::make_unique<DeclarationSpecifiers>(std::move(type), std::move(specifier));
    }
    return std::make_unique<DeclarationSpecifiers>(std::move(type), nullptr);
}

// FIRST(TYPE_QUALIFIER)=  const restrict volatile
// FIRST(FUNCTION_SPECIFIER) = inline
// FIRST(STORAGE_SPECIFIER) =	auto register static

// DECLARATION_SPECIFIERS_PRIME -> TYPE_QUALIFIER | FUNCTION_SPECIFIER | STORAGE_SPECIFIER | ϵ
std::unique_ptr<Specifier> Parser::parseDECLARATION_SPECIFIERS_PRIME()
{
    if (currentToken.getType() == TokenType::KEYWORD)
    {
        const std::string &tokenValue = currentToken.getValue();
        if (tokenValue == "const" || tokenValue == "restrict" || tokenValue == "volatile")
        {
            return parseTYPE_QUALIFIER();
        }
        else if (tokenValue == "inline")
        {
            return parseFUNCTION_SPECIFIER();
        }
        else if (tokenValue == "auto" || tokenValue == "register" || tokenValue == "static")
        {
            return parseSTORAGE_SPECIFIER();
        }
    }
    return nullptr;
}

// FIRST(DIRECT_DECLARATOR) = 	( IDENTIFIER
// FIRST(POINTER) = *
// DECLARATOR -> POINTER DIRECT_DECLARATOR | DIRECT_DECLARATOR
std::unique_ptr<Declarator> Parser::parseDECLARATOR()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*")
    {
        std::unique_ptr<Declarator> declarator = std::make_unique<Declarator>();
        declarator->setPointer(parsePOINTER());
        parseDIRECT_DECLARATOR(declarator);
        return declarator;
    }
    else if ((currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(") || currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::unique_ptr<Declarator> declarator = std::make_unique<Declarator>();
        parseDIRECT_DECLARATOR(declarator);
        return declarator;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected POINTER or DIRECT_DECLARATOR");
    }
}

// DIRECT_DECLARATOR -> IDENTIFIER DIRECT_DECLARATOR_PRIME | ( DECLARATOR ) DIRECT_DECLARATOR_PRIME
void Parser::parseDIRECT_DECLARATOR(std::unique_ptr<Declarator> &declarator)
{
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        declarator->setIdentifier(currentToken.getValue());
        consume();
        parseDIRECT_DECLARATOR_PRIME(declarator);
    }
    // for function pointer declaration
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
    {
        consume();

        std::unique_ptr<Declarator> functionDeclarator = parseDECLARATOR();
        if (functionDeclarator != nullptr)
        {
            declarator->setDeclarator(std::move(functionDeclarator));
        }

        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {

            throw std::runtime_error("Syntax error: Expected closing ) in declarator");
        }
        consume();
        parseDIRECT_DECLARATOR_PRIME(declarator);
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected IDENTIFIER or ( in declarator");
    }
}

// DIRECT_DECLARATOR_PRIME -> [ STATIC_OPT_FAC TYPE_QUALIFIER_LIST_OPT_FAC	ASSIGNMENT_EXPRESSION_OPT_FAC ] DIRECT_DECLARATOR_PRIME | ( PARAMETER_LIST_OPT_FAC ) DIRECT_DECLARATOR_PRIME | ϵ
void Parser::parseDIRECT_DECLARATOR_PRIME(std::unique_ptr<Declarator> &declarator)
{
    while (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "[" || currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
    {
        // in array definition
        if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "[")
        {
            consume();
            arraySize arraySize;
            arraySize.isStatic = parseSTATIC_OPT_FAC();
            std::unique_ptr<Specifier> typeQualifier = parseTYPE_QUALIFIER_LIST_OPT_FAC();
            if (typeQualifier != nullptr)
            {
                arraySize.typeQualifier = typeQualifier->getSpecifier();
            }

            arraySize.expr = parseASSIGNMENT_EXPRESSION_OPT_FAC();
            if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "]")
            {
                throw std::runtime_error("Syntax error: Expected closing ] in declarator");
            }
            consume();
            declarator->addArraySize(std::move(arraySize));
            declarator->setIsArray(true);
        }
        // in function definition
        else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
        {
            consume();
            parsePARAMETER_LIST_OPT_FAC();
            if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
            {
                throw std::runtime_error("Syntax error: Expected closing ) in declarator");
            }
            consume();
            declarator->setFunction(true);
        }
    }
}

// EQUALITY_EXPRESSION -> RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseEQUALITY_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseRELATIONAL_EXPRESSION();

    // EQUALITY_EXPRESSION_PRIME -> == RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME | != RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMPARISON_OPERATOR)
    {
        if (currentToken.getValue() == "==")
        {
            consume();
            std::unique_ptr<Expr> rhs = parseRELATIONAL_EXPRESSION();
            lhs = std::make_unique<BinaryExpr>(std::move(lhs), "==", std::move(rhs));
        }
        else if (currentToken.getValue() == "!=")
        {
            consume();
            std::unique_ptr<Expr> rhs = parseRELATIONAL_EXPRESSION();
            lhs = std::make_unique<BinaryExpr>(std::move(lhs), "!=", std::move(rhs));
        }
        else
        {
            break;
        }
    }
    return lhs;
}

// EQUALITY_EXPRESSION_PRIME -> == RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME | != RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseEQUALITY_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::COMPARISON_OPERATOR)
    {
        if (currentToken.getValue() == "==")
        {
            consume();
            parseRELATIONAL_EXPRESSION();
            parseEQUALITY_EXPRESSION_PRIME();
        }
        if (currentToken.getValue() == "!=")
        {
            consume();
            parseRELATIONAL_EXPRESSION();
            parseEQUALITY_EXPRESSION_PRIME();
        }
    }
    return nullptr;
}

// EXPRESSION -> ASSIGNMENT_EXPRESSION EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseEXPRESSION()
{
    parseASSIGNMENT_EXPRESSION();
    parseEXPRESSION_PRIME();
    return nullptr;
}

// EXPRESSION_PRIME -> , ASSIGNMENT_EXPRESSION EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseEXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parseASSIGNMENT_EXPRESSION();
        parseEXPRESSION_PRIME();
    }
    return nullptr;
}

// EXPRESSION_STATEMENT -> ; | EXPRESSION ;
std::unique_ptr<Expr> Parser::parseEXPRESSION_STATEMENT()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        consume();
        // empty statement
    }
    else if (isExpressionFirst())
    {
        std::unique_ptr<Expr> expression = parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; at the end of an expression");
        }
        consume();
        return expression;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected closing semicolon ; or expression.");
    }

    return nullptr;
}

// FIRST(EXPRESSION_STATEMENT)=    ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// FOR_INIT_STATEMENT -> EXPRESSION_STATEMENT EXPRESSION_STATEMENT FOR_OPTIONAL_EXPRESSION | DECLARATION EXPRESSION_STATEMENT FOR_OPTIONAL_EXPRESSION
std::unique_ptr<ForInitStatement> Parser::parseFOR_INIT_STATEMENT()
{
    if (isDeclarationFirst())
    {
        std::unique_ptr<Declaration> declaration = parseDECLARATION();
        std::unique_ptr<Expr> expression = parseEXPRESSION_STATEMENT();
        std::unique_ptr<Expr> optionalExpression = parseFOR_OPTIONAL_EXPRESSION();
        return std::make_unique<ForWitDeclaration>(std::move(declaration), std::move(expression), std::move(optionalExpression));
    }
    else if (isExpressionFirst() || currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        return std::make_unique<ForWithExpression>(parseEXPRESSION_STATEMENT(), parseEXPRESSION_STATEMENT(), parseFOR_OPTIONAL_EXPRESSION());
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected EXPRESSION_STATEMENT or DECLARATION.");
    }
    return nullptr;
}

// FOR_OPTIONAL_EXPRESSION -> EXPRESSION | ϵ
std::unique_ptr<Expr> Parser::parseFOR_OPTIONAL_EXPRESSION()
{
    if (isExpressionFirst())
    {
        return parseEXPRESSION();
    }
    return nullptr;
}

// FUNCTION_DEFINITION -> func DECLARATION_SPECIFIERS DECLARATOR FUNCTION_DEF_FAC
std::unique_ptr<FunctionDecl> Parser::parseFUNCTION_DEFINITION()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "func")
    {
        consume();
        std::unique_ptr<DeclarationSpecifiers> declarationSpecifiers = parseDECLARATION_SPECIFIERS();
        std::unique_ptr<Declarator> declarator = parseDECLARATOR();
        std::unique_ptr<BlockStmt> block = parseFUNCTION_DEF_FAC();
        std::unique_ptr<FunctionDecl> functionDecl = std::make_unique<FunctionDecl>(
            std::move(declarationSpecifiers),
            std::move(declarator),
            std::move(block));
        return functionDecl;
    }

    return nullptr;
}

// FIRST(DECLARATION_LIST)=	    bool char double float int long short signed struct unsigned void
// FIRST(BLOCK) = {
//  FUNCTION_DEF_FAC -> DECLARATION_LIST BLOCK | BLOCK
std::unique_ptr<BlockStmt> Parser::parseFUNCTION_DEF_FAC()
{
    if (isBasicType())
    {
        parseDECLARATION_LIST();
        return parseBLOCK();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        return parseBLOCK();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected declarations or block {}");
    }

    return nullptr;
}

// FUNCTION_SPECIFIER -> inline
std::unique_ptr<Specifier> Parser::parseFUNCTION_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "inline")
    {
        // save it here
        consume();
        return std::make_unique<Specifier>(currentToken.getValue(), "function");
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected function specifier Token");
    }

    return nullptr;
}

// IDENTIFIER_LIST -> IDENTIFIER IDENTIFIER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseIDENTIFIER_LIST()
{
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        consume();
        parseIDENTIFIER_LIST_PRIME();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected identifier");
    }

    return nullptr;
}

// IDENTIFIER_LIST_PRIME -> , IDENTIFIER IDENTIFIER_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseIDENTIFIER_LIST_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        if (currentToken.getType() == TokenType::IDENTIFIER)
        {
            consume();
            parseIDENTIFIER_LIST_PRIME();
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected identifier");
        }
    }

    return nullptr;
}

// INITIALIZER -> INITIALIZER_BRACE_FAC | ASSIGNMENT_EXPRESSION
std::unique_ptr<Initializer> Parser::parseINITIALIZER()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        std::unique_ptr<BraceInitializer> braceInitializer = std::make_unique<BraceInitializer>(std::move(parseINITIALIZER_BRACE_FAC()));
        return braceInitializer;
    }
    else if (isExpressionFirst())
    {
        std::unique_ptr<ExpressionInitializer> ExpInitializer = std::make_unique<ExpressionInitializer>(std::move(parseASSIGNMENT_EXPRESSION()));
        return ExpInitializer;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected initializer");
    }
    return nullptr;
}

// INITIALIZER_BRACE_FAC -> { INITIALIZER_LIST }
std::unique_ptr<InitializerList> Parser::parseINITIALIZER_BRACE_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        consume();
        std::unique_ptr<InitializerList> initializer = parseINITIALIZER_LIST();
        if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "}")
        {
            consume();
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected closing } in initializer");
        }
        return initializer;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected opening { in initializer");
    }

    return nullptr;
}

// INITIALIZER_LIST -> INITIALIZER INITIALIZER_LIST_PRIME
std::unique_ptr<InitializerList> Parser::parseINITIALIZER_LIST()
{
    std::vector<std::unique_ptr<Initializer>> initializers;
    initializers.push_back(parseINITIALIZER());
    // INITIALIZER_LIST_PRIME -> , INITIALIZER INITIALIZER_LIST_PRIME | ϵ
    while (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        initializers.push_back(parseINITIALIZER());
    }
    return std::make_unique<InitializerList>(std::move(initializers));

    return nullptr;
}

// INIT_DECLARATOR -> DECLARATOR INIT_DECLARATOR_FAC
std::unique_ptr<Declarator> Parser::parseINIT_DECLARATOR()
{
    std::unique_ptr<Declarator> declarator = parseDECLARATOR();
    if (declarator == nullptr)
    {
        throw std::runtime_error("Syntax error: Expected declarator");
    }
    declarator->setInitializer(parseINIT_DECLARATOR_FAC());
    return declarator;
}

// INIT_DECLARATOR_FAC -> = INITIALIZER | ϵ
std::unique_ptr<Initializer> Parser::parseINIT_DECLARATOR_FAC()
{
    if (currentToken.getType() == TokenType::ASSIGNMENT_OPERATOR && currentToken.getValue() == "=")
    {
        consume();
        return parseINITIALIZER();
    }

    return nullptr;
}

// INIT_DECLARATOR_LIST -> INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME
std::unique_ptr<DeclatatorList> Parser::parseINIT_DECLARATOR_LIST()
{
    std::vector<std::unique_ptr<Declarator>> declarators;
    declarators.push_back(std::move(parseINIT_DECLARATOR()));

    // INIT_DECLARATOR_LIST_PRIME -> , INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME | ϵ
    while (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        declarators.push_back(std::move(parseINIT_DECLARATOR()));
    }
    return std::make_unique<DeclatatorList>(std::move(declarators));
}

// ITERATION_STATEMENT -> while ( EXPRESSION ) STATEMENT | do STATEMENT while ( EXPRESSION ) ; | for ( FOR_INIT_STATEMENT ) STATEMENT
std::unique_ptr<IterationStatement> Parser::parseITERATION_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "while")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in while statement");
        }

        consume();
        std::unique_ptr<Expr> expr = parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in while statement");
        }
        consume();
        return std::make_unique<WhileStatement>(std::move(expr), parseSTATEMENT());
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "do")
    {
        consume();
        std::unique_ptr<Stmt> statement = parseSTATEMENT();
        if (currentToken.getType() != TokenType::KEYWORD || currentToken.getValue() != "while")
        {
            throw std::runtime_error("Syntax error: Expected while in do-while statement");
        }
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in do-while statement");
        }
        consume();
        std::unique_ptr<Expr> expr = parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in do-while statement");
        }
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; in do-while statement");
        }
        consume();
        return std::make_unique<DoWhileStatement>(std::move(expr), std::move(statement));
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "for")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in for statement");
        }
        consume();
        std::unique_ptr<ForInitStatement> forInitStatement = parseFOR_INIT_STATEMENT();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in for statement");
        }
        consume();
        return std::make_unique<ForStatement>(std::move(forInitStatement), parseSTATEMENT());
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected while, do or for statement");
    }

    return nullptr;
}

// JUMP_STATEMENT -> continue ; | break ; | return EXPRESSION_STATEMENT
std::unique_ptr<JumpStatement> Parser::parseJUMP_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "continue")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; in continue statement");
        }
        consume();
        return std::make_unique<ContinueStatement>();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "break")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; in break statement");
        }
        consume();
        return std::make_unique<BreakStatement>();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "return")
    {
        consume();
        return std::make_unique<ReturnStatement>(std::move(parseEXPRESSION_STATEMENT()));
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected continue, break or return statement");
    }

    return nullptr;
}

// LABELED_STATEMENT -> case CONDITIONAL_EXPRESSION : STATEMENT | default : STATEMENT
std::unique_ptr<LabaledStatement> Parser::parseLABELED_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "case")
    {
        consume();
        std::unique_ptr<Expr> expr = parseCONDITIONAL_EXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ":")
        {
            throw std::runtime_error("Syntax error: Expected : in case statement");
        }
        consume();
        std::unique_ptr<Stmt> statement = parseSTATEMENT();
        return std::make_unique<CaseStatement>(std::move(expr), std::move(statement));
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "default")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ":")
        {
            throw std::runtime_error("Syntax error: Expected : in default statement");
        }
        consume();
        std::unique_ptr<Stmt> statement = parseSTATEMENT();
        return std::make_unique<DefaultStatement>(std::move(statement));
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected case or default statement");
    }

    return nullptr;
}

// EXCLUSIVE_OR_EXPRESSION -> AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseEXCLUSIVE_OR_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseAND_EXPRESSION();

    // EXCLUSIVE_OR_EXPRESSION_PRIME -> ^ AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "^")
    {
        consume();
        std::unique_ptr<Expr> rhs = parseAND_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "^", std::move(rhs));
    }
    return lhs;
}

// INCLUSIVE_OR_EXPRESSION -> EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseINCLUSIVE_OR_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseEXCLUSIVE_OR_EXPRESSION();
    while (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "|")
    {
        consume();
        std::unique_ptr<Expr> rhs = parseEXCLUSIVE_OR_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "|", std::move(rhs));
    }
    return lhs;
}

// LOGICAL_AND_EXPRESSION -> INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseLOGICAL_AND_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseINCLUSIVE_OR_EXPRESSION();

    // LOGICAL_AND_EXPRESSION_PRIME -> && INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMPARISON_OPERATOR && currentToken.getValue() == "&&")
    {
        consume();
        std::unique_ptr<Expr> rhs = parseINCLUSIVE_OR_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "&&", std::move(rhs));
    }
    return lhs;
}

// LOGICAL_OR_EXPRESSION -> LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseLOGICAL_OR_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseLOGICAL_AND_EXPRESSION();

    // LOGICAL_OR_EXPRESSION_PRIME -> || LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMPARISON_OPERATOR && currentToken.getValue() == "||")
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseLOGICAL_AND_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

// ADDITIVE_EXPRESSION -> MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseADDITIVE_EXPRESSION()
{
    //  ( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
    std::unique_ptr<Expr> lhs = parseMULTIPLICATIVE_EXPRESSION();

    // ADDITIVE_EXPRESSION_PRIME -> + MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME | - MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && (currentToken.getValue() == "+" || currentToken.getValue() == "-"))
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseMULTIPLICATIVE_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs));
    }
    return lhs;
}

// AND_EXPRESSION -> EQUALITY_EXPRESSION AND_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseAND_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseEQUALITY_EXPRESSION();

    // AND_EXPRESSION_PRIME -> & EQUALITY_EXPRESSION AND_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "&")
    {
        consume();
        std::unique_ptr<Expr> rhs = parseEQUALITY_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), "&", std::move(rhs));
    }
    return lhs;
}

// MULTIPLICATIVE_EXPRESSION -> UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseMULTIPLICATIVE_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseUNARY_EXPRESSION();
    // MULTIPLICATIVE_EXPRESSION_PRIME -> * UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | / UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | % UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && (currentToken.getValue() == "*" || currentToken.getValue() == "/" || currentToken.getValue() == "%"))
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseUNARY_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs));
    }
    return nullptr;
}

// FIRST(CONSTANT)=  INTEGER_CONSTANT FLOAT_CONSTANT STRING_LITERAL
//  PRIMARY_EXPRESSION -> IDENTIFIER | CONSTANT | ( EXPRESSION )
std::unique_ptr<PrimaryExpression> Parser::parsePRIMARY_EXPRESSION()
{
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        std::string identifier = currentToken.getValue();
        consume();
        return std::make_unique<IdentifierExpression>(identifier);
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
    {
        consume();
        std::unique_ptr<ParenthesizedExpression> parenthesizedExpr = std::make_unique<ParenthesizedExpression>(std::move(parseEXPRESSION()));
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in primary expression");
        }
        consume();
        return parenthesizedExpr;
    }
    else if (currentToken.getType() == TokenType::INTEGER_CONSTANT || currentToken.getType() == TokenType::FLOAT_CONSTANT || currentToken.getType() == TokenType::STRING_LITERAL)
    {
        return parseCONSTANT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected identifier, constant or expression in primary expression");
    }

    return nullptr;
}

// RELATIONAL_EXPRESSION -> SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseRELATIONAL_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseSHIFT_EXPRESSION();

    // RELATIONAL_EXPRESSION_PRIME -> < SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | > SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | <= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | >= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | ϵ
    while (currentToken.getType() == TokenType::COMPARISON_OPERATOR)
    {
        if (currentToken.getValue() == "<" || currentToken.getValue() == ">" || currentToken.getValue() == "<=" || currentToken.getValue() == ">=")
        {
            std::string op = currentToken.getValue();
            consume();
            std::unique_ptr<Expr> rhs = parseSHIFT_EXPRESSION();
            lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs));
        }
        else
        {
            break;
        }
    }
    return lhs;
}

// PARAMETER_DECLARATION -> DECLARATION_SPECIFIERS PARAM_DECL_FAC
std::unique_ptr<ASTNode> Parser::parsePARAMETER_DECLARATION()
{
    parseDECLARATION_SPECIFIERS();
    parsePARAM_DECL_FAC();

    return nullptr;
}

// PARAMETER_LIST -> PARAMETER_DECLARATION PARAMETER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST()
{
    parsePARAMETER_DECLARATION();
    parsePARAMETER_LIST_PRIME();

    return nullptr;
}

// FIRST(IDENTIFIER_LIST)=  IDENTIFIER
//  FIRST(PARAMETER_LIST)=  bool char double float int long short signed struct unsigned void
//  PARAMETER_LIST_OPT_FAC -> PARAMETER_LIST | IDENTIFIER_LIST | ϵ
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST_OPT_FAC()
{
    if (isBasicType())
    {
        parsePARAMETER_LIST();
    }
    else if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        parseIDENTIFIER_LIST();
    }
    return nullptr;
}

// PARAMETER_LIST_PRIME -> , PARAMETER_DECLARATION PARAMETER_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parsePARAMETER_DECLARATION();
        parsePARAMETER_LIST_PRIME();
    }

    return nullptr;
}

// FIRST(DECLARATOR)=     ( * IDENTIFIER
// PARAM_DECL_FAC -> DECLARATOR | ϵ
std::unique_ptr<ASTNode> Parser::parsePARAM_DECL_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(" || currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*" || currentToken.getType() == TokenType::IDENTIFIER)
    {
        parseDECLARATOR();
    }
    return nullptr;
}

// POINTER -> *
bool Parser::parsePOINTER()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*")
    {
        consume();
        return true;
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected * in pointer");
    }
    return false;
}

// FIRST(ARGUMENT_EXPRESSION_LIST) = 	( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// POSTFIX_ARGUMENTS -> ARGUMENT_EXPRESSION_LIST | ϵ
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_ARGUMENTS()
{
    if (isExpressionFirst())
    {
        return parseARGUMENT_EXPRESSION_LIST();
    }

    return nullptr;
}

// POSTFIX_EXPRESSION -> PRIMARY_EXPRESSION POSTFIX_EXPRESSION_PRIME
std::unique_ptr<PostfixExpression> Parser::parsePOSTFIX_EXPRESSION()
{
    std::unique_ptr<PrimaryExpression> primaryExpr = parsePRIMARY_EXPRESSION();

    // POSTFIX_EXPRESSION_PRIME -> [ EXPRESSION ] POSTFIX_EXPRESSION_PRIME | ( POSTFIX_ARGUMENTS ) POSTFIX_EXPRESSION_PRIME | . IDENTIFIER POSTFIX_EXPRESSION_PRIME | -> IDENTIFIER POSTFIX_EXPRESSION_PRIME | ++ POSTFIX_EXPRESSION_PRIME | -- POSTFIX_EXPRESSION_PRIME | ϵ
    while (true)
    {
        if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "[")
        {
            consume();
            std::unique_ptr<ArrayPostFixExpression> arrayPostFixExpr = std::make_unique<ArrayPostFixExpression>(parseEXPRESSION());
            // parseEXPRESSION();
            if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "]")
            {
                throw std::runtime_error("Syntax error: Expected closing ] in postfix expression");
            }
            consume();
        }
        else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
        {
            consume();
            parsePOSTFIX_ARGUMENTS();
            if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
            {
                throw std::runtime_error("Syntax error: Expected closing ) in postfix expression");
            }
            consume();
        }
        else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ".")
        {
            consume();
            if (currentToken.getType() != TokenType::IDENTIFIER)
            {
                throw std::runtime_error("Syntax error: Expected identifier after . in postfix expression");
            }
            consume();
        }
        else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "->")
        {
            consume();
            if (currentToken.getType() != TokenType::IDENTIFIER)
            {
                throw std::runtime_error("Syntax error: Expected identifier after -> in postfix expression");
            }
            consume();
        }
        else if (currentToken.getType() == TokenType::INCREMENT_OPERATOR && (currentToken.getValue() == "++" || currentToken.getValue() == "--"))
        {
            consume();
        }
        else
        {
            break;
        }
    }
    return nullptr;
}

// SELECTION_STATEMENT -> if ( EXPRESSION ) STATEMENT | switch ( EXPRESSION ) STATEMENT
std::unique_ptr<SelectionStatement> Parser::parseSELECTION_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "if")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in if statement");
        }
        consume();
        std::unique_ptr<Expr> expression = parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in if statement");
        }
        consume();
        return std::make_unique<IfStatement>(std::move(expression), std::move(parseSTATEMENT()));
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "switch")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in switch statement");
        }
        consume();
        std::unique_ptr<Expr> expression = parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in switch statement");
        }
        consume();
        return std::make_unique<SwitchStatement>(std::move(expression), std::move(parseSTATEMENT()));
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected if or switch statement");
    }

    return nullptr;
}

// SHIFT_EXPRESSION -> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME
std::unique_ptr<Expr> Parser::parseSHIFT_EXPRESSION()
{
    std::unique_ptr<Expr> lhs = parseADDITIVE_EXPRESSION();

    // SHIFT_EXPRESSION_PRIME -> << ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | >> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | ϵ
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && (currentToken.getValue() == "<<" || currentToken.getValue() == ">>"))
    {
        std::string op = currentToken.getValue();
        consume();
        std::unique_ptr<Expr> rhs = parseADDITIVE_EXPRESSION();
        lhs = std::make_unique<BinaryExpr>(std::move(lhs), op, std::move(rhs));
    }
    return lhs;
}

// SPECIFIER_QUALIFIER_LIST -> TYPE_SPECIFIER
std::unique_ptr<ASTNode> Parser::parseSPECIFIER_QUALIFIER_LIST()
{
    parseTYPE_SPECIFIER();
    return nullptr;
}

// FIRST(LABALED_STATEMENT)=  case default
// FIRST(BLOCK)= {
// FIRST(EXPRESSION_STATEMENT)=  ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// FIRST(SELECTION_STATEMENT)=  if switch
// FIRST(ITERATION_STATEMENT)=  while do for
// FIRST(JUMP_STATEMENT)=  continue break return
//  STATEMENT -> LABELED_STATEMENT | BLOCK | EXPRESSION_STATEMENT | SELECTION_STATEMENT | ITERATION_STATEMENT | JUMP_STATEMENT
std::unique_ptr<Stmt> Parser::parseSTATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "case" || currentToken.getValue() == "default")
    {
        return parseLABELED_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        return parseBLOCK();
    }
    else if (isExpressionFirst() || currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        std::unique_ptr<Expr> expression = parseEXPRESSION_STATEMENT();
        return std::make_unique<ExpressionStatement>(std::move(expression));
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "if" || currentToken.getValue() == "switch")
    {
        return parseSELECTION_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "while" || currentToken.getValue() == "do" || currentToken.getValue() == "for")
    {
        return parseITERATION_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "continue" || currentToken.getValue() == "break" || currentToken.getValue() == "return")
    {
        return parseJUMP_STATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected statement");
    }

    return nullptr;
}

// STATIC_OPT_FAC -> static | ϵ
bool Parser::parseSTATIC_OPT_FAC()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "static")
    {
        true;
        consume();
    }

    return false;
}

// STORAGE_SPECIFIER -> static | auto | register
std::unique_ptr<Specifier> Parser::parseSTORAGE_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "static" || currentToken.getValue() == "auto" || currentToken.getValue() == "register"))
    {
        consume();
        return std::make_unique<Specifier>(currentToken.getValue(), "Storage");
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected storage specifier Token");
    }

    return nullptr;
}

// STRUCT_DECLARATION -> SPECIFIER_QUALIFIER_LIST STRUCT_DECL_FAC
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION()
{
    parseSPECIFIER_QUALIFIER_LIST();
    parseSTRUCT_DECL_FAC();
    return nullptr;
}

// STRUCT_DECLARATION_LIST -> STRUCT_DECLARATION STRUCT_DECLARATION_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION_LIST()
{
    parseSTRUCT_DECLARATION();
    parseSTRUCT_DECLARATION_LIST_PRIME();
    return nullptr;
}

// FIRST(STRUCT_DECLARATION) = bool char double float int long short signed struct unsigned void
// STRUCT_DECLARATION_LIST_PRIME -> STRUCT_DECLARATION STRUCT_DECLARATION_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION_LIST_PRIME()
{
    if (isBasicType())
    {
        parseSTRUCT_DECLARATION();
        parseSTRUCT_DECLARATION_LIST_PRIME();
    }
    return nullptr;
}

// STRUCT_DECLARATOR_LIST -> DECLARATOR STRUCT_DECLARATOR_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATOR_LIST()
{
    parseDECLARATOR();
    parseSTRUCT_DECLARATOR_LIST_PRIME();
    return nullptr;
}

// STRUCT_DECLARATOR_LIST_PRIME -> , DECLARATOR STRUCT_DECLARATOR_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATOR_LIST_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parseDECLARATOR();
        parseSTRUCT_DECLARATOR_LIST_PRIME();
    }
    return nullptr;
}

// FIRST(STRUCT_DECLARATOR_LIST)=    ( * IDENTIFIER
//  STRUCT_DECL_FAC -> ; | STRUCT_DECLARATOR_LIST ;
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECL_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        consume();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(" || currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*" || currentToken.getType() == TokenType::IDENTIFIER)
    {
        parseSTRUCT_DECLARATOR_LIST();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; in struct declaration");
        }
        consume();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected ; or struct declarator list");
    }

    return nullptr;
}

// STRUCT_SPECIFIER -> struct STRUCT_SPEC_FAC
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "struct")
    {
        consume();
        parseSTRUCT_SPEC_FAC();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected 'struct' specifier Token");
    }
    return nullptr;
}

// STRUCT_SPEC_FAC -> { STRUCT_DECLARATION_LIST } | IDENTIFIER STRUCT_SPEC_FAC2
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPEC_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        consume();
        parseSTRUCT_DECLARATION_LIST();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "}")
        {
            throw std::runtime_error("Syntax error: Expected closing } in struct specifier");
        }
        consume();
    }
    else if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        consume();
        parseSTRUCT_SPEC_FAC2();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected { or IDENTIFIER in struct specifier");
    }
    return nullptr;
}

// STRUCT_SPEC_FAC2 -> { STRUCT_DECLARATION_LIST } | ϵ
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPEC_FAC2()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        consume();
        parseSTRUCT_DECLARATION_LIST();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "}")
        {
            throw std::runtime_error("Syntax error: Expected closing } in struct specifier");
        }
        consume();
    }
    return nullptr;
}

// TYPE_QUALIFIER -> const | restrict | volatile
std::unique_ptr<Specifier> Parser::parseTYPE_QUALIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "const" || currentToken.getValue() == "restrict" || currentToken.getValue() == "volatile"))
    {
        consume();
        return std::make_unique<Specifier>(currentToken.getValue(), "Qualifier");
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected type qualifier Token");
    }

    return nullptr;
}

// TYPE_QUALIFIER_LIST_OPT_FAC -> TYPE_QUALIFIER | ϵ
std::unique_ptr<Specifier> Parser::parseTYPE_QUALIFIER_LIST_OPT_FAC()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "const" || currentToken.getValue() == "restrict" || currentToken.getValue() == "volatile"))
    {
        return parseTYPE_QUALIFIER();
    }
    return nullptr;
}

// TYPE_SPECIFIER -> int | void | char | short | long | float | double | signed | unsigned | bool | STRUCT_SPECIFIER
std::unique_ptr<Type> Parser::parseTYPE_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "int" || currentToken.getValue() == "void" || currentToken.getValue() == "char" || currentToken.getValue() == "short" || currentToken.getValue() == "long" || currentToken.getValue() == "float" || currentToken.getValue() == "double" || currentToken.getValue() == "signed" || currentToken.getValue() == "unsigned" || currentToken.getValue() == "bool"))
    {
        std::string type = currentToken.getValue();
        consume();
        return std::make_unique<Type>(type);
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "struct")
    {
        parseSTRUCT_SPECIFIER();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected type specifier Token");
    }

    return nullptr;
}

// FIRST(POSTFIX_EXPRESSION)=     ( FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL

// UNARY_EXPRESSION -> POSTFIX_EXPRESSION | ++ UNARY_EXPRESSION | -- UNARY_EXPRESSION | sizeof UNARY_EXPRESSION
std::unique_ptr<Expr> Parser::parseUNARY_EXPRESSION()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(" || currentToken.getType() == TokenType::FLOAT_CONSTANT || currentToken.getType() == TokenType::IDENTIFIER || currentToken.getType() == TokenType::INTEGER_CONSTANT || currentToken.getType() == TokenType::STRING_LITERAL)
    {
        parsePOSTFIX_EXPRESSION();
    }
    else if (currentToken.getType() == TokenType::INCREMENT_OPERATOR && (currentToken.getValue() == "++" || currentToken.getValue() == "--"))
    {
        consume();
        parseUNARY_EXPRESSION();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "sizeof")
    {
        consume();
        parseUNARY_EXPRESSION();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected postfix expression, ++, -- or sizeof");
    }

    return nullptr;
}