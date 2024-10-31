#include <stdexcept>
#include "Parser.h"
#include <unordered_set>

/*
HELPER FUNCTIONS
*/
bool Parser::isBasicType()
{
    std::string tokenValue = currentToken.getValue();
    return tokenValue == "bool" || tokenValue == "char" || tokenValue == "double" ||
           tokenValue == "float" || tokenValue == "int" || tokenValue == "long" ||
           tokenValue == "short" || tokenValue == "signed" || tokenValue == "struct" ||
           tokenValue == "unsigned" || tokenValue == "void";
}

bool Parser::isFirstTokenForProgramPrime()
{
    if (currentToken.getType() != TokenType::KEYWORD)
    {
        return false; // If it's not a keyword, return false
    }

    const std::unordered_set<std::string> validKeywords = {
        "bool", "char", "double", "float", "func",
        "int", "long", "short", "signed", "struct",
        "unsigned", "void"};

    // Check if the current token's value is in the set of valid keywords
    return validKeywords.find(currentToken.getValue()) != validKeywords.end();
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
    programNode->push_back_vec(parsePROGRAM_PRIME());

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    return programNode;
}

// PROGRAM_PRIME -> GLOBAL_DECLARATIONS PROGRAM_PRIME | ϵ
std::unique_ptr<ListNode> Parser::parsePROGRAM_PRIME()
{
    auto programPrimeNode = std::make_unique<GlobalDeclarationsNode>();

    // Check if the current token is the first token for PROGRAM_PRIME
    while (isFirstTokenForProgramPrime()) // Continue as long as there are valid tokens
    {
        // Parse another set of global declarations
        auto additionalDeclarations = parseGlobalDeclarations();
        programPrimeNode->push_back_node(std::move(additionalDeclarations));
    }

    return programPrimeNode;
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

std::unique_ptr<ASTNode> Parser::parseADDITIVE_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseADDITIVE_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseAND_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseAND_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseARGUMENT_EXPRESSION_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseARGUMENT_EXPRESSION_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_EXPRESSION_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_EXPRESSION_OPT_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseASSIGNMENT_OPERATOR() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseBLOCK() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseBLOCK_CONTENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseBLOCK_ITEM() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseBLOCK_ITEM_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseBLOCK_ITEM_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseCONDITIONAL_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseCONDITIONAL_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseCONSTANT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATION_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATION_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATION_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATION_SPECIFIERS() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATION_SPECIFIERS_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDECLARATOR() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDIRECT_DECLARATOR() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseDIRECT_DECLARATOR_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEQUALITY_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEQUALITY_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEXCLUSIVE_OR_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEXCLUSIVE_OR_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseEXPRESSION_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseFOR_INIT_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseFOR_OPTIONAL_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseFUNCTION_DEFINITION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseFUNCTION_DEF_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseFUNCTION_SPECIFIER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseGLOBAL_DECLARATIONS() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseIDENTIFIER_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseIDENTIFIER_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINCLUSIVE_OR_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINCLUSIVE_OR_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINITIALIZER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_BRACE_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINITIALIZER_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseINIT_DECLARATOR_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseITERATION_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseJUMP_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseLABELED_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseLOGICAL_AND_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseLOGICAL_AND_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseLOGICAL_OR_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseLOGICAL_OR_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseMULTIPLICATIVE_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseMULTIPLICATIVE_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePARAMETER_DECLARATION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST_OPT_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePARAMETER_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePARAM_DECL_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePOINTER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_ARGUMENTS() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePOSTFIX_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parsePRIMARY_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseRELATIONAL_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseRELATIONAL_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSELECTION_STATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSHIFT_EXPRESSION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSHIFT_EXPRESSION_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSPECIFIER_QUALIFIER_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTATEMENT() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTATIC_OPT_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTORAGE_SPECIFIER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATION_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATOR_LIST() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECLARATOR_LIST_PRIME() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_DECL_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPECIFIER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPEC_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseSTRUCT_SPEC_FAC2() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseTYPE_QUALIFIER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseTYPE_QUALIFIER_LIST_OPT_FAC() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseTYPE_SPECIFIER() { return nullptr; }
std::unique_ptr<ASTNode> Parser::parseUNARY_EXPRESSION() { return nullptr; }
