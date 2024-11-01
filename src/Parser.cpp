#include <stdexcept>
#include "Parser.h"
#include <unordered_set>

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

std::unique_ptr<ASTNode> Parser::parsePROGRAM()
{
    // auto programNode = std::make_unique<ProgramNode>();
    // programNode->push_back_node(parseGLOBAL_DECLARATIONS());
    // programNode->push_back_node(parsePROGRAM_PRIME());
    parseGLOBAL_DECLARATIONS();
    parsePROGRAM_PRIME();

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    return nullptr;
}

// PROGRAM_PRIME -> GLOBAL_DECLARATIONS PROGRAM_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parsePROGRAM_PRIME()
{
    auto programNode = std::make_unique<ProgramNode>();
    // Base Case: If the token is not in FIRST(PROGRAM_PRIME), return an empty ProgramPrimeAST for ε
    if (!isDeclarationFirst())
    {
        return nullptr; // Return empty node for ε
    }

    // Recursive Case: Parse a GLOBAL_DECLARATIONS node and recursively parse remaining PROGRAM_PRIME
    auto programPrimeNode = parseGLOBAL_DECLARATIONS();

    // Recursively parse the next PROGRAM_PRIME
    std::unique_ptr<ASTNode> nextProgramPrime = parsePROGRAM_PRIME();
    return nullptr;
}

// GLOBAL_DECLARATIONS -> DECLARATION | FUNCTION_DEFINITION
std::unique_ptr<ASTNode> Parser::parseGLOBAL_DECLARATIONS()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "func")
    {
        auto declarationNode = std::make_unique<DeclarationNode>();
        declarationNode->setDeclaration(parseFUNCTION_DEFINITION());
        return declarationNode;
    }

    if (isBasicType())
    {
        auto declarationNode = std::make_unique<DeclarationNode>();
        declarationNode->setDeclaration(parseDECLARATION());
        return declarationNode;
    }

    return nullptr;
}

// ADDITIVE_EXPRESSION -> MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseADDITIVE_EXPRESSION()
{
    //  ( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
    parseMULTIPLICATIVE_EXPRESSION();
    parseADDITIVE_EXPRESSION_PRIME();
    return nullptr;
}

// ADDITIVE_EXPRESSION_PRIME -> + MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME | - MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseADDITIVE_EXPRESSION_PRIME()
{
    if (currentToken.getType() != TokenType::ARITHMETIC_OPERATOR)
    {
        return nullptr;
    }
    if (currentToken.getValue() == "+" || currentToken.getValue() == "-")
    {
        consume();
        parseMULTIPLICATIVE_EXPRESSION();
        parseADDITIVE_EXPRESSION_PRIME();
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseAND_EXPRESSION()
{
    parseEQUALITY_EXPRESSION();
    parseAND_EXPRESSION_PRIME();
    return nullptr;
}

// AND_EXPRESSION_PRIME -> & EQUALITY_EXPRESSION AND_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseAND_EXPRESSION_PRIME()
{
    if (currentToken.getType() != TokenType::ARITHMETIC_OPERATOR)
    {
        return nullptr;
    }
    if (currentToken.getValue() != "&")
    {
        consume();
        parseEQUALITY_EXPRESSION();
        parseAND_EXPRESSION_PRIME();
    }
    return nullptr;
}

// ARGUMENT_EXPRESSION_LIST -> ASSIGNMENT_EXPRESSION ARGUMENT_EXPRESSION_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseARGUMENT_EXPRESSION_LIST()
{
    parseASSIGNMENT_EXPRESSION();
    parseARGUMENT_EXPRESSION_LIST_PRIME();
    return nullptr;
}

// ARGUMENT_EXPRESSION_LIST_PRIME -> , ASSIGNMENT_EXPRESSION ARGUMENT_EXPRESSION_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseARGUMENT_EXPRESSION_LIST_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parseASSIGNMENT_EXPRESSION();
        parseARGUMENT_EXPRESSION_LIST_PRIME();
    }
    return nullptr;
}

// ASSIGNMENT_EXPRESSION -> CONDITIONAL_EXPRESSION ASSIGNMENT_EXPRESSION_FAC
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_EXPRESSION()
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
std::unique_ptr<ASTNode> Parser::parseBLOCK()
{

    if ((currentToken.getType() == TokenType::PUNCTUATION) && (currentToken.getValue() == "{"))
    {
        consume();
        parseBLOCK_CONTENT();
        if ((currentToken.getType() == TokenType::PUNCTUATION) && (currentToken.getValue() == "}"))
        {
            consume();
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected '}' at the end of block");
        }
    }

    return nullptr;
}

// BLOCK_CONTENT -> BLOCK_ITEM_LIST | ϵ
std::unique_ptr<ASTNode> Parser::parseBLOCK_CONTENT()
{
    if (isBlockFirst())
    {
        parseBLOCK_ITEM_LIST();
    }
    return nullptr;
}

// BLOCK_ITEM -> DECLARATION | STATEMENT
std::unique_ptr<ASTNode> Parser::parseBLOCK_ITEM()
{
    if (isBasicType())
    {
        parseDECLARATION();
    }
    else if (isStatementFirst())
    {
        parseSTATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected declaration or statement");
    }
    return nullptr;
}

// BLOCK_ITEM_LIST -> BLOCK_ITEM BLOCK_ITEM_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseBLOCK_ITEM_LIST()
{
    parseBLOCK_ITEM();
    parseBLOCK_ITEM_LIST_PRIME();
    return nullptr;
}

// BLOCK_ITEM_LIST_PRIME -> BLOCK_ITEM BLOCK_ITEM_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseBLOCK_ITEM_LIST_PRIME()
{
    if (isBlockFirst())
    {
        parseBLOCK_ITEM();
        parseBLOCK_ITEM_LIST_PRIME();
    }
    return nullptr;
}

// CONDITIONAL_EXPRESSION -> LOGICAL_OR_EXPRESSION CONDITIONAL_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseCONDITIONAL_EXPRESSION()
{
    parseLOGICAL_OR_EXPRESSION();
    parseCONDITIONAL_EXPRESSION_PRIME();
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
std::unique_ptr<ASTNode> Parser::parseCONSTANT()
{
    if (currentToken.getType() == TokenType::INTEGER_CONSTANT)
    {
        consume();
    }
    else if (currentToken.getType() == TokenType::FLOAT_CONSTANT)
    {
        consume();
    }
    else if (currentToken.getType() == TokenType::STRING_LITERAL)
    {
        consume();
    }
    return nullptr;
}

// DECLARATION -> DECLARATION_SPECIFIERS DECLARATION_FAC
std::unique_ptr<ASTNode> Parser::parseDECLARATION()
{
    parseDECLARATION_SPECIFIERS();
    parseDECLARATION_FAC();
    return nullptr;
}

// FIRST(INIT_DECLARATOR_LIST)= 	( * IDENTIFIER
// DECLARATION_FAC -> ; | INIT_DECLARATOR_LIST ;
std::unique_ptr<ASTNode> Parser::parseDECLARATION_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        consume();
    }
    else if (
        (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(") || (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*") || (currentToken.getType() == TokenType::IDENTIFIER))
    {
        parseINIT_DECLARATOR_LIST();
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
std::unique_ptr<ASTNode> Parser::parseDECLARATION_SPECIFIERS()
{
    parseTYPE_SPECIFIER();
    parseDECLARATION_SPECIFIERS_PRIME();
    return nullptr;
}

// FIRST(TYPE_QUALIFIER)=  const restrict volatile
// FIRST(FUNCTION_SPECIFIER) = inline
// FIRST(STORAGE_SPECIFIER) =	auto register static

// DECLARATION_SPECIFIERS_PRIME -> TYPE_QUALIFIER | FUNCTION_SPECIFIER | STORAGE_SPECIFIER | ϵ
std::unique_ptr<ASTNode> Parser::parseDECLARATION_SPECIFIERS_PRIME()
{
    if (currentToken.getType() == TokenType::KEYWORD)
    {
        const std::string &tokenValue = currentToken.getValue();
        if (tokenValue == "const" || tokenValue == "restrict" || tokenValue == "volatile")
        {
            parseTYPE_QUALIFIER();
        }
        else if (tokenValue == "inline")
        {
            parseFUNCTION_SPECIFIER();
        }
        else if (tokenValue == "auto" || tokenValue == "register" || tokenValue == "static")
        {
            parseSTORAGE_SPECIFIER();
        }
    }
    else
    {
        // throw std::runtime_error("Syntax error: Expected declaration specifier Token, declaration specifier prime. Got: " + currentToken.getValue());
    }

    return nullptr;
}

// FIRST(DIRECT_DECLARATOR) = 	( IDENTIFIER
// FIRST(POINTER) = *
// DECLARATOR -> POINTER DIRECT_DECLARATOR | DIRECT_DECLARATOR
std::unique_ptr<ASTNode> Parser::parseDECLARATOR()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "*")
    {
        parsePOINTER();
        parseDIRECT_DECLARATOR();
    }
    else if ((currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(") || currentToken.getType() == TokenType::IDENTIFIER)
    {
        parseDIRECT_DECLARATOR();
    }
    return nullptr;
}

// DIRECT_DECLARATOR -> IDENTIFIER DIRECT_DECLARATOR_PRIME | ( DECLARATOR ) DIRECT_DECLARATOR_PRIME
std::unique_ptr<ASTNode> Parser::parseDIRECT_DECLARATOR()
{
    if (currentToken.getType() == TokenType::IDENTIFIER)
    {
        consume();
        parseDIRECT_DECLARATOR_PRIME();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
    {
        consume();
        parseDECLARATOR();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {

            throw std::runtime_error("Syntax error: Expected closing ) in declarator");
        }
        consume();
        parseDIRECT_DECLARATOR_PRIME();
    }
    return nullptr;
}

// DIRECT_DECLARATOR_PRIME -> [ STATIC_OPT_FAC TYPE_QUALIFIER_LIST_OPT_FAC	ASSIGNMENT_EXPRESSION_OPT_FAC ] DIRECT_DECLARATOR_PRIME | ( PARAMETER_LIST_OPT_FAC ) DIRECT_DECLARATOR_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseDIRECT_DECLARATOR_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "[")
    {
        consume();
        parseSTATIC_OPT_FAC();
        parseTYPE_QUALIFIER_LIST_OPT_FAC();
        parseASSIGNMENT_EXPRESSION_OPT_FAC();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "]")
        {
            throw std::runtime_error("Syntax error: Expected closing ] in declarator");
        }
        consume();
        parseDIRECT_DECLARATOR_PRIME();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
    {
        consume();
        parsePARAMETER_LIST_OPT_FAC();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in declarator");
        }
        consume();
        parseDIRECT_DECLARATOR_PRIME();
    }
    return nullptr;
}

// EQUALITY_EXPRESSION -> RELATIONAL_EXPRESSION EQUALITY_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseEQUALITY_EXPRESSION()
{
    parseRELATIONAL_EXPRESSION();
    parseEQUALITY_EXPRESSION_PRIME();
    return nullptr;
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

// EXCLUSIVE_OR_EXPRESSION -> AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseEXCLUSIVE_OR_EXPRESSION()
{
    parseAND_EXPRESSION();
    parseEXCLUSIVE_OR_EXPRESSION_PRIME();
    return nullptr;
}

// EXCLUSIVE_OR_EXPRESSION_PRIME -> ^ AND_EXPRESSION EXCLUSIVE_OR_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseEXCLUSIVE_OR_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "^")
    {
        parseAND_EXPRESSION();
        parseEXCLUSIVE_OR_EXPRESSION_PRIME();
    }

    return nullptr;
}

// EXPRESSION -> ASSIGNMENT_EXPRESSION EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseEXPRESSION()
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
std::unique_ptr<ASTNode> Parser::parseEXPRESSION_STATEMENT()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        consume();
    }
    else if (isExpressionFirst())
    {
        parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; at the end of an expression");
        }
        consume();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected closing semicolon ; or expression.");
    }

    return nullptr;
}

// FIRST(EXPRESSION_STATEMENT)=    ( ++ -- ; FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// FOR_INIT_STATEMENT -> EXPRESSION_STATEMENT EXPRESSION_STATEMENT FOR_OPTIONAL_EXPRESSION | DECLARATION EXPRESSION_STATEMENT FOR_OPTIONAL_EXPRESSION
std::unique_ptr<ASTNode> Parser::parseFOR_INIT_STATEMENT()
{
    if (isDeclarationFirst())
    {
        parseDECLARATION();
        parseEXPRESSION_STATEMENT();
        parseFOR_OPTIONAL_EXPRESSION();
    }
    else if (isExpressionFirst() || currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        parseEXPRESSION_STATEMENT();
        parseEXPRESSION_STATEMENT();
        parseFOR_OPTIONAL_EXPRESSION();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected EXPRESSION_STATEMENT or DECLARATION.");
    }
    return nullptr;
}

// FOR_OPTIONAL_EXPRESSION -> EXPRESSION | ϵ
std::unique_ptr<ASTNode> Parser::parseFOR_OPTIONAL_EXPRESSION()
{
    if (isExpressionFirst())
    {
        parseEXPRESSION();
    }
    return nullptr;
}

// FUNCTION_DEFINITION -> func DECLARATION_SPECIFIERS DECLARATOR FUNCTION_DEF_FAC
std::unique_ptr<ASTNode> Parser::parseFUNCTION_DEFINITION()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "func")
    {
        consume();
        parseDECLARATION_SPECIFIERS();
        parseDECLARATOR();
        parseFUNCTION_DEF_FAC();
    }

    return nullptr;
}

// FIRST(DECLARATION_LIST)=	    bool char double float int long short signed struct unsigned void
// FIRST(BLOCK) = {
//  FUNCTION_DEF_FAC -> DECLARATION_LIST BLOCK | BLOCK
std::unique_ptr<ASTNode> Parser::parseFUNCTION_DEF_FAC()
{
    if (isBasicType())
    {
        parseDECLARATION_LIST();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        parseBLOCK();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected declarations or block {}");
    }

    return nullptr;
}

// FUNCTION_SPECIFIER -> inline
std::unique_ptr<ASTNode> Parser::parseFUNCTION_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "inline")
    {
        // save it here
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

// INCLUSIVE_OR_EXPRESSION -> EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseINCLUSIVE_OR_EXPRESSION()
{
    parseEXCLUSIVE_OR_EXPRESSION();
    parseINCLUSIVE_OR_EXPRESSION_PRIME();
    return nullptr;
}

// INCLUSIVE_OR_EXPRESSION_PRIME -> | EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseINCLUSIVE_OR_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && currentToken.getValue() == "|")
    {
        consume();
        parseEXCLUSIVE_OR_EXPRESSION();
        parseINCLUSIVE_OR_EXPRESSION_PRIME();
    }
    return nullptr;
}

// INITIALIZER -> INITIALIZER_BRACE_FAC | ASSIGNMENT_EXPRESSION
std::unique_ptr<ASTNode> Parser::parseINITIALIZER()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        parseINITIALIZER_BRACE_FAC();
    }
    else if (isExpressionFirst())
    {
        parseASSIGNMENT_EXPRESSION();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected initializer");
    }
    return nullptr;
}

// INITIALIZER_BRACE_FAC -> { INITIALIZER_LIST }
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_BRACE_FAC()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        consume();
        parseINITIALIZER_LIST();
        if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "}")
        {
            consume();
        }
        else
        {
            throw std::runtime_error("Syntax error: Expected closing } in initializer");
        }
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected opening { in initializer");
    }

    return nullptr;
}

// INITIALIZER_LIST -> INITIALIZER INITIALIZER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_LIST()
{
    parseINITIALIZER();
    parseINITIALIZER_LIST_PRIME();
    return nullptr;
}

// INITIALIZER_LIST_PRIME -> , INITIALIZER INITIALIZER_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_LIST_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parseINITIALIZER();
        parseINITIALIZER_LIST_PRIME();
    }
    return nullptr;
}

// INIT_DECLARATOR -> DECLARATOR INIT_DECLARATOR_FAC
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR()
{
    parseDECLARATOR();
    parseINIT_DECLARATOR_FAC();
    return nullptr;
}

// INIT_DECLARATOR_FAC -> = INITIALIZER | ϵ
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_FAC()
{
    if (currentToken.getType() == TokenType::ASSIGNMENT_OPERATOR && currentToken.getValue() == "=")
    {
        consume();
        parseINITIALIZER();
    }

    return nullptr;
}

// INIT_DECLARATOR_LIST -> INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_LIST()
{
    parseINIT_DECLARATOR();
    parseINIT_DECLARATOR_LIST_PRIME();
    return nullptr;
}

// INIT_DECLARATOR_LIST_PRIME -> , INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_LIST_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ",")
    {
        consume();
        parseINIT_DECLARATOR();
        parseINIT_DECLARATOR_LIST_PRIME();
    }
    return nullptr;
}

// ITERATION_STATEMENT -> while ( EXPRESSION ) STATEMENT | do STATEMENT while ( EXPRESSION ) ; | for ( FOR_INIT_STATEMENT ) STATEMENT
std::unique_ptr<ASTNode> Parser::parseITERATION_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "while")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in while statement");
        }

        consume();
        parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in while statement");
        }
        consume();
        parseSTATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "do")
    {
        consume();
        parseSTATEMENT();
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
        parseEXPRESSION();
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
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "for")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in for statement");
        }
        consume();
        parseFOR_INIT_STATEMENT();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in for statement");
        }
        consume();
        parseSTATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected while, do or for statement");
    }

    return nullptr;
}

// JUMP_STATEMENT -> continue ; | break ; | return EXPRESSION_STATEMENT
std::unique_ptr<ASTNode> Parser::parseJUMP_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "continue")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; in continue statement");
        }
        consume();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "break")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ";")
        {
            throw std::runtime_error("Syntax error: Expected ; in break statement");
        }
        consume();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "return")
    {
        consume();
        parseEXPRESSION_STATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected continue, break or return statement");
    }

    return nullptr;
}

// LABELED_STATEMENT -> case CONDITIONAL_EXPRESSION : STATEMENT | default : STATEMENT
std::unique_ptr<ASTNode> Parser::parseLABELED_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "case")
    {
        consume();
        parseCONDITIONAL_EXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ":")
        {
            throw std::runtime_error("Syntax error: Expected : in case statement");
        }
        consume();
        parseSTATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "default")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ":")
        {
            throw std::runtime_error("Syntax error: Expected : in default statement");
        }
        consume();
        parseSTATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected case or default statement");
    }

    return nullptr;
}

// LOGICAL_AND_EXPRESSION -> INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseLOGICAL_AND_EXPRESSION()
{
    parseINCLUSIVE_OR_EXPRESSION();
    parseLOGICAL_AND_EXPRESSION_PRIME();
    return nullptr;
}

// LOGICAL_AND_EXPRESSION_PRIME -> && INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseLOGICAL_AND_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::LOGICAL_OPERATOR && currentToken.getValue() == "&&")
    {
        consume();
        parseINCLUSIVE_OR_EXPRESSION();
        parseLOGICAL_AND_EXPRESSION_PRIME();
    }
    return nullptr;
}

// LOGICAL_OR_EXPRESSION -> LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseLOGICAL_OR_EXPRESSION()
{
    parseLOGICAL_AND_EXPRESSION();
    parseLOGICAL_OR_EXPRESSION_PRIME();

    return nullptr;
}

// LOGICAL_OR_EXPRESSION_PRIME -> || LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseLOGICAL_OR_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::COMPARISON_OPERATOR && currentToken.getValue() == "||")
    {
        consume();
        parseLOGICAL_AND_EXPRESSION();
        parseLOGICAL_OR_EXPRESSION_PRIME();
    }

    return nullptr;
}

// MULTIPLICATIVE_EXPRESSION -> UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseMULTIPLICATIVE_EXPRESSION()
{
    parseUNARY_EXPRESSION();
    parseMULTIPLICATIVE_EXPRESSION_PRIME();
    return nullptr;
}

// MULTIPLICATIVE_EXPRESSION_PRIME -> * UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | / UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | % UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseMULTIPLICATIVE_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && (currentToken.getValue() == "*" || currentToken.getValue() == "/" || currentToken.getValue() == "%"))
    {
        consume();
        parseUNARY_EXPRESSION();
        parseMULTIPLICATIVE_EXPRESSION_PRIME();
    }

    return nullptr;
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

// PARAMETER_LIST -> PARAMETER_DECLARATION PARAMETER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parsePOINTER()
{
    parsePARAMETER_DECLARATION();
    parsePARAMETER_LIST_PRIME();
    return nullptr;
}

// FIRST(ARGUMENT_EXPRESSION_LIST) = 	( ++ -- FLOAT_CONSTANT IDENTIFIER INTEGER_CONSTANT STRING_LITERAL sizeof
// POSTFIX_ARGUMENTS -> ARGUMENT_EXPRESSION_LIST | ϵ
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_ARGUMENTS()
{
    if (isExpressionFirst())
    {
        parseARGUMENT_EXPRESSION_LIST();
    }

    return nullptr;
}

// POSTFIX_EXPRESSION -> PRIMARY_EXPRESSION POSTFIX_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_EXPRESSION()
{
    parsePRIMARY_EXPRESSION();
    parsePOSTFIX_EXPRESSION_PRIME();
    return nullptr;
}

// POSTFIX_EXPRESSION_PRIME -> [ EXPRESSION ] POSTFIX_EXPRESSION_PRIME | ( POSTFIX_ARGUMENTS ) POSTFIX_EXPRESSION_PRIME | . IDENTIFIER POSTFIX_EXPRESSION_PRIME | -> IDENTIFIER POSTFIX_EXPRESSION_PRIME | ++ POSTFIX_EXPRESSION_PRIME | -- POSTFIX_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "[")
    {
        consume();
        parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "]")
        {
            throw std::runtime_error("Syntax error: Expected closing ] in postfix expression");
        }
        consume();
        parsePOSTFIX_EXPRESSION_PRIME();
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
        parsePOSTFIX_EXPRESSION_PRIME();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ".")
    {
        consume();
        if (currentToken.getType() != TokenType::IDENTIFIER)
        {
            throw std::runtime_error("Syntax error: Expected identifier after . in postfix expression");
        }
        consume();
        parsePOSTFIX_EXPRESSION_PRIME();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "->")
    {
        consume();
        if (currentToken.getType() != TokenType::IDENTIFIER)
        {
            throw std::runtime_error("Syntax error: Expected identifier after -> in postfix expression");
        }
        consume();
        parsePOSTFIX_EXPRESSION_PRIME();
    }
    else if (currentToken.getType() == TokenType::INCREMENT_OPERATOR && (currentToken.getValue() == "++" || currentToken.getValue() == "--"))
    {
        consume();
        parsePOSTFIX_EXPRESSION_PRIME();
    }

    return nullptr;
}

// FIRST(CONSTANT)=  INTEGER_CONSTANT FLOAT_CONSTANT STRING_LITERAL
//  PRIMARY_EXPRESSION -> IDENTIFIER | CONSTANT | ( EXPRESSION )
std::unique_ptr<ASTNode> Parser::parsePRIMARY_EXPRESSION()
{
    if (currentToken.getType() == TokenType::IDENTIFIER || currentToken.getType() == TokenType::INTEGER_CONSTANT || currentToken.getType() == TokenType::FLOAT_CONSTANT || currentToken.getType() == TokenType::STRING_LITERAL)
    {
        consume();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "(")
    {
        consume();
        parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in primary expression");
        }
        consume();
    }
    else if (currentToken.getType() == TokenType::INTEGER_CONSTANT || currentToken.getType() == TokenType::FLOAT_CONSTANT || currentToken.getType() == TokenType::STRING_LITERAL)
    {
        parseCONSTANT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected identifier, constant or expression in primary expression");
    }

    return nullptr;
}

// RELATIONAL_EXPRESSION -> SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseRELATIONAL_EXPRESSION()
{
    parseSHIFT_EXPRESSION();
    parseRELATIONAL_EXPRESSION_PRIME();
    return nullptr;
}

// RELATIONAL_EXPRESSION_PRIME -> < SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | > SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | <= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | >= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseRELATIONAL_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::COMPARISON_OPERATOR)
    {
        if (currentToken.getValue() == "<" || currentToken.getValue() == ">" || currentToken.getValue() == "<=" || currentToken.getValue() == ">=")
        {
            consume();
            parseSHIFT_EXPRESSION();
            parseRELATIONAL_EXPRESSION_PRIME();
        }
    }
    return nullptr;
}

// SELECTION_STATEMENT -> if ( EXPRESSION ) STATEMENT | switch ( EXPRESSION ) STATEMENT
std::unique_ptr<ASTNode> Parser::parseSELECTION_STATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "if")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in if statement");
        }
        consume();
        parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in if statement");
        }
        consume();
        parseSTATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "switch")
    {
        consume();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != "(")
        {
            throw std::runtime_error("Syntax error: Expected opening ( in switch statement");
        }
        consume();
        parseEXPRESSION();
        if (currentToken.getType() != TokenType::PUNCTUATION || currentToken.getValue() != ")")
        {
            throw std::runtime_error("Syntax error: Expected closing ) in switch statement");
        }
        consume();
        parseSTATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected if or switch statement");
    }

    return nullptr;
}

// SHIFT_EXPRESSION -> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseSHIFT_EXPRESSION()
{
    parseADDITIVE_EXPRESSION();
    parseSHIFT_EXPRESSION_PRIME();
    return nullptr;
}

// SHIFT_EXPRESSION_PRIME -> << ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | >> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseSHIFT_EXPRESSION_PRIME()
{
    if (currentToken.getType() == TokenType::ARITHMETIC_OPERATOR && (currentToken.getValue() == "<<" || currentToken.getValue() == ">>"))
    {
        consume();
        parseADDITIVE_EXPRESSION();
        parseSHIFT_EXPRESSION_PRIME();
    }

    return nullptr;
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
std::unique_ptr<ASTNode> Parser::parseSTATEMENT()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "case" || currentToken.getValue() == "default")
    {
        parseLABELED_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == "{")
    {
        parseBLOCK();
    }
    else if (isExpressionFirst() || currentToken.getType() == TokenType::PUNCTUATION && currentToken.getValue() == ";")
    {
        parseEXPRESSION_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "if" || currentToken.getValue() == "switch")
    {
        parseSELECTION_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "while" || currentToken.getValue() == "do" || currentToken.getValue() == "for")
    {
        parseITERATION_STATEMENT();
    }
    else if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "continue" || currentToken.getValue() == "break" || currentToken.getValue() == "return")
    {
        parseJUMP_STATEMENT();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected statement");
    }

    return nullptr;
}

// STATIC_OPT_FAC -> static | ϵ
std::unique_ptr<ASTNode> Parser::parseSTATIC_OPT_FAC()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "static")
    {
        consume();
    }

    return nullptr;
}

// STORAGE_SPECIFIER -> static | auto | register
std::unique_ptr<ASTNode> Parser::parseSTORAGE_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "static" || currentToken.getValue() == "auto" || currentToken.getValue() == "register"))
    {
        consume();
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
std::unique_ptr<ASTNode> Parser::parseTYPE_QUALIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "const" || currentToken.getValue() == "restrict" || currentToken.getValue() == "volatile"))
    {
        consume();
    }
    else
    {
        throw std::runtime_error("Syntax error: Expected type qualifier Token");
    }

    return nullptr;
}

// TYPE_QUALIFIER_LIST_OPT_FAC -> TYPE_QUALIFIER | ϵ
std::unique_ptr<ASTNode> Parser::parseTYPE_QUALIFIER_LIST_OPT_FAC()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "const" || currentToken.getValue() == "restrict" || currentToken.getValue() == "volatile"))
    {
        parseTYPE_QUALIFIER();
    }
    return nullptr;
}

// TYPE_SPECIFIER -> int | void | char | short | long | float | double | signed | unsigned | bool | STRUCT_SPECIFIER
std::unique_ptr<ASTNode> Parser::parseTYPE_SPECIFIER()
{
    if (currentToken.getType() == TokenType::KEYWORD && (currentToken.getValue() == "int" || currentToken.getValue() == "void" || currentToken.getValue() == "char" || currentToken.getValue() == "short" || currentToken.getValue() == "long" || currentToken.getValue() == "float" || currentToken.getValue() == "double" || currentToken.getValue() == "signed" || currentToken.getValue() == "unsigned" || currentToken.getValue() == "bool"))
    {
        consume();
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
std::unique_ptr<ASTNode> Parser::parseUNARY_EXPRESSION()
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