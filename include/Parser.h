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

    std::unique_ptr<ASTNode> parsePROGRAM(); // this is the starting point

private:
    TokenTable &tokens;
    Token currentToken;

    void consume();
    void expect(TokenType expected); // consume el token si es del tipo que queremos

    std::unique_ptr<ASTNode> parseGlobalDeclarations();

    std::unique_ptr<ASTNode> parseADDITIVE_EXPRESSION();
    std::unique_ptr<ASTNode> parseADDITIVE_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseAND_EXPRESSION();
    std::unique_ptr<ASTNode> parseAND_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseARGUMENT_EXPRESSION_LIST();
    std::unique_ptr<ASTNode> parseARGUMENT_EXPRESSION_LIST_PRIME();
    std::unique_ptr<ASTNode> parseASSIGNMENT_EXPRESSION();
    std::unique_ptr<ASTNode> parseASSIGNMENT_EXPRESSION_FAC();
    std::unique_ptr<ASTNode> parseASSIGNMENT_EXPRESSION_OPT_FAC();
    std::unique_ptr<ASTNode> parseASSIGNMENT_OPERATOR();
    std::unique_ptr<ASTNode> parseBLOCK();
    std::unique_ptr<ASTNode> parseBLOCK_CONTENT();
    std::unique_ptr<ASTNode> parseBLOCK_ITEM();
    std::unique_ptr<ASTNode> parseBLOCK_ITEM_LIST();
    std::unique_ptr<ASTNode> parseBLOCK_ITEM_LIST_PRIME();
    std::unique_ptr<ASTNode> parseCONDITIONAL_EXPRESSION();
    std::unique_ptr<ASTNode> parseCONDITIONAL_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseCONSTANT();
    std::unique_ptr<ASTNode> parseDECLARATION();
    std::unique_ptr<ASTNode> parseDECLARATION_FAC();
    std::unique_ptr<ASTNode> parseDECLARATION_LIST();
    std::unique_ptr<ASTNode> parseDECLARATION_LIST_PRIME();
    std::unique_ptr<ASTNode> parseDECLARATION_SPECIFIERS();
    std::unique_ptr<ASTNode> parseDECLARATION_SPECIFIERS_PRIME();
    std::unique_ptr<ASTNode> parseDECLARATOR();
    std::unique_ptr<ASTNode> parseDIRECT_DECLARATOR();
    std::unique_ptr<ASTNode> parseDIRECT_DECLARATOR_PRIME();
    std::unique_ptr<ASTNode> parseEQUALITY_EXPRESSION();
    std::unique_ptr<ASTNode> parseEQUALITY_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseEXCLUSIVE_OR_EXPRESSION();
    std::unique_ptr<ASTNode> parseEXCLUSIVE_OR_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseEXPRESSION();
    std::unique_ptr<ASTNode> parseEXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseEXPRESSION_STATEMENT();
    std::unique_ptr<ASTNode> parseFOR_INIT_STATEMENT();
    std::unique_ptr<ASTNode> parseFOR_OPTIONAL_EXPRESSION();
    std::unique_ptr<ASTNode> parseFUNCTION_DEFINITION();
    std::unique_ptr<ASTNode> parseFUNCTION_DEF_FAC();
    std::unique_ptr<ASTNode> parseFUNCTION_SPECIFIER();
    std::unique_ptr<ASTNode> parseGLOBAL_DECLARATIONS();
    std::unique_ptr<ASTNode> parseIDENTIFIER_LIST();
    std::unique_ptr<ASTNode> parseIDENTIFIER_LIST_PRIME();
    std::unique_ptr<ASTNode> parseINCLUSIVE_OR_EXPRESSION();
    std::unique_ptr<ASTNode> parseINCLUSIVE_OR_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseINITIALIZER();
    std::unique_ptr<ASTNode> parseINITIALIZER_BRACE_FAC();
    std::unique_ptr<ASTNode> parseINITIALIZER_LIST();
    std::unique_ptr<ASTNode> parseINITIALIZER_LIST_PRIME();
    std::unique_ptr<ASTNode> parseINIT_DECLARATOR();
    std::unique_ptr<ASTNode> parseINIT_DECLARATOR_FAC();
    std::unique_ptr<ASTNode> parseINIT_DECLARATOR_LIST();
    std::unique_ptr<ASTNode> parseINIT_DECLARATOR_LIST_PRIME();
    std::unique_ptr<ASTNode> parseITERATION_STATEMENT();
    std::unique_ptr<ASTNode> parseJUMP_STATEMENT();
    std::unique_ptr<ASTNode> parseLABELED_STATEMENT();
    std::unique_ptr<ASTNode> parseLOGICAL_AND_EXPRESSION();
    std::unique_ptr<ASTNode> parseLOGICAL_AND_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseLOGICAL_OR_EXPRESSION();
    std::unique_ptr<ASTNode> parseLOGICAL_OR_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseMULTIPLICATIVE_EXPRESSION();
    std::unique_ptr<ASTNode> parseMULTIPLICATIVE_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parsePARAMETER_DECLARATION();
    std::unique_ptr<ASTNode> parsePARAMETER_LIST();
    std::unique_ptr<ASTNode> parsePARAMETER_LIST_OPT_FAC();
    std::unique_ptr<ASTNode> parsePARAMETER_LIST_PRIME();
    std::unique_ptr<ASTNode> parsePARAM_DECL_FAC();
    std::unique_ptr<ASTNode> parsePOINTER();
    std::unique_ptr<ASTNode> parsePOSTFIX_ARGUMENTS();
    std::unique_ptr<ASTNode> parsePOSTFIX_EXPRESSION();
    std::unique_ptr<ASTNode> parsePOSTFIX_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parsePRIMARY_EXPRESSION();
    std::unique_ptr<ListNode> parsePROGRAM_PRIME();
    std::unique_ptr<ASTNode> parseRELATIONAL_EXPRESSION();
    std::unique_ptr<ASTNode> parseRELATIONAL_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseSELECTION_STATEMENT();
    std::unique_ptr<ASTNode> parseSHIFT_EXPRESSION();
    std::unique_ptr<ASTNode> parseSHIFT_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseSPECIFIER_QUALIFIER_LIST();
    std::unique_ptr<ASTNode> parseSTATEMENT();
    std::unique_ptr<ASTNode> parseSTATIC_OPT_FAC();
    std::unique_ptr<ASTNode> parseSTORAGE_SPECIFIER();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATION();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATION_LIST();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATION_LIST_PRIME();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATOR_LIST();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATOR_LIST_PRIME();
    std::unique_ptr<ASTNode> parseSTRUCT_DECL_FAC();
    std::unique_ptr<ASTNode> parseSTRUCT_SPECIFIER();
    std::unique_ptr<ASTNode> parseSTRUCT_SPEC_FAC();
    std::unique_ptr<ASTNode> parseSTRUCT_SPEC_FAC2();
    std::unique_ptr<ASTNode> parseTYPE_QUALIFIER();
    std::unique_ptr<ASTNode> parseTYPE_QUALIFIER_LIST_OPT_FAC();
    std::unique_ptr<ASTNode> parseTYPE_SPECIFIER();
    std::unique_ptr<ASTNode> parseUNARY_EXPRESSION();

    bool isFirstTokenForProgramPrime();
    bool isBasicType();
};

/*

*/
