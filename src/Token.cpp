#include "Token.h"

std::string Token::getValue() const
{
    return value;
}

TokenType Token::getType() const
{
    return type;
}

std::string Token::getTypeAsString() const
{
    switch (type)
    {
    case TokenType::KEYWORD:
        return "KEYWORD";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::PUNCTUATION:
        return "PUNCTUATION";
    case TokenType::INTEGER_CONSTANT:
        return "INTEGER_CONSTANT";
    case TokenType::FLOAT_CONSTANT:
        return "FLOAT_CONSTANT";
    case TokenType::STRING_LITERAL:
        return "STRING_LITERAL";
    case TokenType::UNKNOWN:
        return "UNKNOWN";
    case TokenType::ARITHMETIC_OPERATOR:
        return "ARITHMETIC_OPERATOR";
    case TokenType::COMPARISON_OPERATOR:
        return "COMPARISON_OPERATOR";
    case TokenType::ASSIGNMENT_OPERATOR:
        return "ASSIGNMENT_OPERATOR";
    case TokenType::INCREMENT_OPERATOR:
        return "INCREMENT_OPERATOR";
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    default:
        return "INVALID_TYPE";
    }
}
