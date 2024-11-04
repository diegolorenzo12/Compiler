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

    std::unique_ptr<ASTNode> parseGlobalDeclarations();

    std::unique_ptr<ASTNode> parseADDITIVE_EXPRESSION();
    std::unique_ptr<ASTNode> parseADDITIVE_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseAND_EXPRESSION();
    std::unique_ptr<ASTNode> parseAND_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseARGUMENT_EXPRESSION_LIST();
    std::unique_ptr<ASTNode> parseARGUMENT_EXPRESSION_LIST_PRIME();
    std::unique_ptr<Expr> parseASSIGNMENT_EXPRESSION();
    std::unique_ptr<ASTNode> parseASSIGNMENT_EXPRESSION_FAC();
    std::unique_ptr<ASTNode> parseASSIGNMENT_EXPRESSION_OPT_FAC();
    std::unique_ptr<ASTNode> parseASSIGNMENT_OPERATOR();
    std::unique_ptr<BlockStmt> parseBLOCK();
    std::unique_ptr<BlockStmt> parseBLOCK_CONTENT();
    std::unique_ptr<BlockItemBase> parseBLOCK_ITEM();
    std::unique_ptr<BlockStmt> parseBLOCK_ITEM_LIST();
    std::unique_ptr<ASTNode> parseBLOCK_ITEM_LIST_PRIME();
    std::unique_ptr<ASTNode> parseCONDITIONAL_EXPRESSION();
    std::unique_ptr<ASTNode> parseCONDITIONAL_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseCONSTANT();
    std::unique_ptr<Declaration> parseDECLARATION();
    std::unique_ptr<DeclatatorList> parseDECLARATION_FAC();
    std::unique_ptr<ASTNode> parseDECLARATION_LIST();
    std::unique_ptr<ASTNode> parseDECLARATION_LIST_PRIME();
    std::unique_ptr<DeclarationSpecifiers> parseDECLARATION_SPECIFIERS();
    std::unique_ptr<Specifier> parseDECLARATION_SPECIFIERS_PRIME();
    std::unique_ptr<Declarator> parseDECLARATOR();
    void parseDIRECT_DECLARATOR(std::unique_ptr<Declarator> &declarator);
    // std::unique_ptr<DirectDeclarator> parseDIRECT_DECLARATOR_PRIME();
    void parseDIRECT_DECLARATOR_PRIME(std::unique_ptr<Declarator> &declarato);
    std::unique_ptr<ASTNode> parseEQUALITY_EXPRESSION();
    std::unique_ptr<ASTNode> parseEQUALITY_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseEXCLUSIVE_OR_EXPRESSION();
    std::unique_ptr<ASTNode> parseEXCLUSIVE_OR_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseEXPRESSION();
    std::unique_ptr<ASTNode> parseEXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseEXPRESSION_STATEMENT();
    std::unique_ptr<ASTNode> parseFOR_INIT_STATEMENT();
    std::unique_ptr<ASTNode> parseFOR_OPTIONAL_EXPRESSION();
    std::unique_ptr<FunctionDecl> parseFUNCTION_DEFINITION();
    std::unique_ptr<BlockStmt> parseFUNCTION_DEF_FAC();
    std::unique_ptr<Specifier> parseFUNCTION_SPECIFIER();
    std::unique_ptr<ASTNode> parseGLOBAL_DECLARATIONS();
    std::unique_ptr<ASTNode> parseIDENTIFIER_LIST();
    std::unique_ptr<ASTNode> parseIDENTIFIER_LIST_PRIME();
    std::unique_ptr<ASTNode> parseINCLUSIVE_OR_EXPRESSION();
    std::unique_ptr<ASTNode> parseINCLUSIVE_OR_EXPRESSION_PRIME();
    std::unique_ptr<Initializer> parseINITIALIZER();
    std::unique_ptr<InitializerList> parseINITIALIZER_BRACE_FAC();
    std::unique_ptr<InitializerList> parseINITIALIZER_LIST();
    std::unique_ptr<ASTNode> parseINITIALIZER_LIST_PRIME();
    std::unique_ptr<Declarator> parseINIT_DECLARATOR();
    std::unique_ptr<Initializer> parseINIT_DECLARATOR_FAC();
    std::unique_ptr<DeclatatorList> parseINIT_DECLARATOR_LIST();
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
    bool parsePOINTER();
    std::unique_ptr<ASTNode> parsePOSTFIX_ARGUMENTS();
    std::unique_ptr<ASTNode> parsePOSTFIX_EXPRESSION();
    std::unique_ptr<ASTNode> parsePOSTFIX_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parsePRIMARY_EXPRESSION();
    std::unique_ptr<ASTNode> parseRELATIONAL_EXPRESSION();
    std::unique_ptr<ASTNode> parseRELATIONAL_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseSELECTION_STATEMENT();
    std::unique_ptr<ASTNode> parseSHIFT_EXPRESSION();
    std::unique_ptr<ASTNode> parseSHIFT_EXPRESSION_PRIME();
    std::unique_ptr<ASTNode> parseSPECIFIER_QUALIFIER_LIST();
    std::unique_ptr<ASTNode> parseSTATEMENT();
    bool parseSTATIC_OPT_FAC();
    std::unique_ptr<Specifier> parseSTORAGE_SPECIFIER();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATION();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATION_LIST();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATION_LIST_PRIME();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATOR_LIST();
    std::unique_ptr<ASTNode> parseSTRUCT_DECLARATOR_LIST_PRIME();
    std::unique_ptr<ASTNode> parseSTRUCT_DECL_FAC();
    std::unique_ptr<ASTNode> parseSTRUCT_SPECIFIER();
    std::unique_ptr<ASTNode> parseSTRUCT_SPEC_FAC();
    std::unique_ptr<ASTNode> parseSTRUCT_SPEC_FAC2();
    std::unique_ptr<Specifier> parseTYPE_QUALIFIER();
    std::unique_ptr<Specifier> parseTYPE_QUALIFIER_LIST_OPT_FAC();
    std::unique_ptr<Type> parseTYPE_SPECIFIER();
    std::unique_ptr<ASTNode> parseUNARY_EXPRESSION();
};

/*

*/
