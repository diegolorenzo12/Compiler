#include "Token.h"


std::string Token::getValue() const {
    return value;
}

TokenType Token::getType() const {
    return type;
}



std::string Token::getTypeAsString() const {
    switch (type) {
    case TokenType::KEYWORD: return "KEYWORD";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::PUNCTUATION: return "PUNCTUATION";
    case TokenType::OPERATOR: return "OPERATOR";
    case TokenType::CONSTANT: return "CONSTANT";
    case TokenType::STRING_LITERAL: return "STRING_LITERAL";
    case TokenType::COMMENT: return "COMMENT";
    default:
        return "UNKNOWN";
    }
}
