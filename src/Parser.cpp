#include <stdexcept>
#include "Parser.h"

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

std::unique_ptr<ASTNode> Parser::parseProgram()
{
    auto programNode = std::make_unique<ProgramNode>();
    programNode->push_back_node(parseGlobalDeclarations());
    programNode->push_back_node(parseProgramPrime());

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    return programNode;
}
