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

    std::unique_ptr<ASTNode> parseProgram();

private:
    TokenTable &tokens; // Reference to the TokenTable
    Token currentToken;

    void consume(TokenType expectedType);
    void advance();
    void expect(TokenType expected); // consume el token si es del tipo que queremos

    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseFuncDef();
};
