#include <stdexcept>
#include "Parser.h"

void Parser::advance()
{
}

void Parser::expect(TokenType expected)
{
    if (currentToken.getType() != expected)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    advance(); // Move to the next token
}