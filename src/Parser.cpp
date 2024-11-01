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
    auto programNode = std::make_unique<ProgramNode>();
    programNode->push_back_node(parseGLOBAL_DECLARATIONS());
    programNode->push_back_node(parsePROGRAM_PRIME());

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    return programNode;
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

// AND_EXPRESSION -> EQUALITY_EXPRESSION AND_EXPRESSION_PRIME
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
        if ((currentToken.getType() == TokenType::PUNCTUATION) && (currentToken.getValue() == "{"))
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
    if (currentToken.getType() != TokenType::KEYWORD)
    {
        throw std::runtime_error("Syntax error: Expected declaration specifier Token");
    }
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
    else
    {
        throw std::runtime_error("Syntax error: Expected declaration specifier Token");
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
        throw std::runtime_error("Syntax error: Expected declaration specifier Token");
    }

    return nullptr;
}

// IDENTIFIER_LIST -> IDENTIFIER IDENTIFIER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseIDENTIFIER_LIST() { return nullptr; }

// IDENTIFIER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseIDENTIFIER_LIST_PRIME() { return nullptr; }

// INCLUSIVE_OR_EXPRESSION -> EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseINCLUSIVE_OR_EXPRESSION() { return nullptr; }

// INCLUSIVE_OR_EXPRESSION_PRIME -> ~ EXCLUSIVE_OR_EXPRESSION INCLUSIVE_OR_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseINCLUSIVE_OR_EXPRESSION_PRIME() { return nullptr; }

// INITIALIZER -> INITIALIZER_BRACE_FAC | ASSIGNMENT_EXPRESSION
std::unique_ptr<ASTNode> Parser::parseINITIALIZER() { return nullptr; }

// INITIALIZER_BRACE_FAC -> { INITIALIZER_LIST }
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_BRACE_FAC() { return nullptr; }

// INITIALIZER_LIST -> INITIALIZER INITIALIZER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_LIST() { return nullptr; }

// INITIALIZER_LIST_PRIME -> , INITIALIZER INITIALIZER_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_LIST_PRIME() { return nullptr; }

// INIT_DECLARATOR -> DECLARATOR INIT_DECLARATOR_FAC
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR() { return nullptr; }

// INIT_DECLARATOR_FAC -> = INITIALIZER | ϵ
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_FAC() { return nullptr; }

// INIT_DECLARATOR_LIST -> INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_LIST() { return nullptr; }

// INIT_DECLARATOR_LIST_PRIME -> , INIT_DECLARATOR INIT_DECLARATOR_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_LIST_PRIME() { return nullptr; }

// ITERATION_STATEMENT -> while ( EXPRESSION ) STATEMENT | do STATEMENT while ( EXPRESSION ) ; | for ( FOR_INIT_STATEMENT ) STATEMENT
std::unique_ptr<ASTNode> Parser::parseITERATION_STATEMENT() { return nullptr; }

// JUMP_STATEMENT -> continue ; | break ; | return EXPRESSION_STATEMENT
std::unique_ptr<ASTNode> Parser::parseJUMP_STATEMENT() { return nullptr; }

// LABELED_STATEMENT -> case CONDITIONAL_EXPRESSION : STATEMENT | default : STATEMENT
std::unique_ptr<ASTNode> Parser::parseLABELED_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseLOGICAL_AND_EXPRESSION() { return nullptr; }

// LOGICAL_AND_EXPRESSION_PRIME -> && INCLUSIVE_OR_EXPRESSION LOGICAL_AND_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseLOGICAL_AND_EXPRESSION_PRIME() { return nullptr; }

// LOGICAL_OR_EXPRESSION -> LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseLOGICAL_OR_EXPRESSION() { return nullptr; }

// LOGICAL_OR_EXPRESSION_PRIME -> ~~ LOGICAL_AND_EXPRESSION LOGICAL_OR_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseLOGICAL_OR_EXPRESSION_PRIME() { return nullptr; }

// MULTIPLICATIVE_EXPRESSION -> UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseMULTIPLICATIVE_EXPRESSION() { return nullptr; }

// MULTIPLICATIVE_EXPRESSION_PRIME -> * UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | / UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | % UNARY_EXPRESSION MULTIPLICATIVE_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseMULTIPLICATIVE_EXPRESSION_PRIME() { return nullptr; }

// PARAMETER_DECLARATION -> DECLARATION_SPECIFIERS PARAM_DECL_FAC
std::unique_ptr<ASTNode> Parser::parsePARAMETER_DECLARATION() { return nullptr; }

// PARAMETER_LIST -> PARAMETER_DECLARATION PARAMETER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST() { return nullptr; }

// PARAMETER_LIST_OPT_FAC -> PARAMETER_LIST | IDENTIFIER_LIST | ϵ
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST_OPT_FAC() { return nullptr; }

// PARAMETER_LIST_PRIME -> , PARAMETER_DECLARATION PARAMETER_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST_PRIME() { return nullptr; }

// PARAM_DECL_FAC -> DECLARATOR | ϵ
std::unique_ptr<ASTNode> Parser::parsePARAM_DECL_FAC() { return nullptr; }

// PARAMETER_LIST -> PARAMETER_DECLARATION PARAMETER_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parsePOINTER() { return nullptr; }

// POSTFIX_ARGUMENTS -> ARGUMENT_EXPRESSION_LIST | ϵ
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_ARGUMENTS() { return nullptr; }

// POSTFIX_EXPRESSION -> PRIMARY_EXPRESSION POSTFIX_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_EXPRESSION() { return nullptr; }

// POSTFIX_EXPRESSION_PRIME -> [ EXPRESSION ] POSTFIX_EXPRESSION_PRIME | ( POSTFIX_ARGUMENTS ) POSTFIX_EXPRESSION_PRIME | . IDENTIFIER POSTFIX_EXPRESSION_PRIME | ~> IDENTIFIER POSTFIX_EXPRESSION_PRIME | ++ POSTFIX_EXPRESSION_PRIME | -- POSTFIX_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_EXPRESSION_PRIME() { return nullptr; }

// PRIMARY_EXPRESSION -> IDENTIFIER | CONSTANT | ( EXPRESSION )
std::unique_ptr<ASTNode> Parser::parsePRIMARY_EXPRESSION() { return nullptr; }

// RELATIONAL_EXPRESSION -> SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseRELATIONAL_EXPRESSION() { return nullptr; }

// RELATIONAL_EXPRESSION_PRIME -> < SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | > SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | <= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | >= SHIFT_EXPRESSION RELATIONAL_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseRELATIONAL_EXPRESSION_PRIME() { return nullptr; }

// SELECTION_STATEMENT -> if ( EXPRESSION ) STATEMENT | switch ( EXPRESSION ) STATEMENT
std::unique_ptr<ASTNode> Parser::parseSELECTION_STATEMENT() { return nullptr; }

// SHIFT_EXPRESSION -> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME
std::unique_ptr<ASTNode> Parser::parseSHIFT_EXPRESSION() { return nullptr; }

// SHIFT_EXPRESSION_PRIME -> << ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | >> ADDITIVE_EXPRESSION SHIFT_EXPRESSION_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseSHIFT_EXPRESSION_PRIME() { return nullptr; }

// SPECIFIER_QUALIFIER_LIST -> TYPE_SPECIFIER
std::unique_ptr<ASTNode> Parser::parseSPECIFIER_QUALIFIER_LIST() { return nullptr; }

// STATEMENT -> LABELED_STATEMENT | BLOCK | EXPRESSION_STATEMENT | SELECTION_STATEMENT | ITERATION_STATEMENT | JUMP_STATEMENT
std::unique_ptr<ASTNode> Parser::parseSTATEMENT() { return nullptr; }

// STATIC_OPT_FAC -> static | ϵ
std::unique_ptr<ASTNode> Parser::parseSTATIC_OPT_FAC() { return nullptr; }

// STORAGE_SPECIFIER -> static | auto | register
std::unique_ptr<ASTNode> Parser::parseSTORAGE_SPECIFIER() { return nullptr; }

// STRUCT_DECLARATION -> SPECIFIER_QUALIFIER_LIST STRUCT_DECL_FAC
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION() { return nullptr; }

// STRUCT_DECLARATION_LIST
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION_LIST() { return nullptr; }

// STRUCT_DECLARATION_LIST_PRIME -> STRUCT_DECLARATION STRUCT_DECLARATION_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION_LIST_PRIME() { return nullptr; }

// STRUCT_DECLARATOR_LIST -> DECLARATOR STRUCT_DECLARATOR_LIST_PRIME
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATOR_LIST() { return nullptr; }

// STRUCT_DECLARATOR_LIST_PRIME -> , DECLARATOR STRUCT_DECLARATOR_LIST_PRIME | ϵ
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATOR_LIST_PRIME() { return nullptr; }

// STRUCT_DECL_FAC -> ; | STRUCT_DECLARATOR_LIST ;
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECL_FAC() { return nullptr; }

// STRUCT_SPECIFIER -> struct STRUCT_SPEC_FAC
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPECIFIER() { return nullptr; }

// STRUCT_SPEC_FAC -> { STRUCT_DECLARATION_LIST } | IDENTIFIER STRUCT_SPEC_FAC2
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPEC_FAC() { return nullptr; }

// STRUCT_SPEC_FAC2 -> { STRUCT_DECLARATION_LIST } | ϵ
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPEC_FAC2() { return nullptr; }

// TYPE_QUALIFIER -> const | restrict | volatile
std::unique_ptr<ASTNode> Parser::parseTYPE_QUALIFIER() { return nullptr; }

// TYPE_QUALIFIER_LIST_OPT_FAC -> TYPE_QUALIFIER | ϵ
std::unique_ptr<ASTNode> Parser::parseTYPE_QUALIFIER_LIST_OPT_FAC() { return nullptr; }

// TYPE_SPECIFIER -> int | void | char | short | long | float | double | signed | unsigned | bool | STRUCT_SPECIFIER
std::unique_ptr<ASTNode> Parser::parseTYPE_SPECIFIER() { return nullptr; }

// UNARY_EXPRESSION -> POSTFIX_EXPRESSION | ++ UNARY_EXPRESSION | -- UNARY_EXPRESSION | sizeof UNARY_EXPRESSION
std::unique_ptr<ASTNode> Parser::parseUNARY_EXPRESSION() { return nullptr; }
