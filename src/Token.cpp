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
        case TokenType::CONSTANT: return "CONSTANT";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::ARITHMETIC_OPERATOR: return "ARITHMETIC_OPERATOR";
        case TokenType::LOGICAL_OPERATOR: return "LOGICAL_OPERATOR";
        case TokenType::COMPARISON_OPERATOR: return "COMPARISON_OPERATOR";
        case TokenType::ASSIGNMENT_OPERATOR: return "ASSIGNMENT_OPERATOR";
        case TokenType::INCREMENT_DECREMENT_OPERATOR: return "INCREMENT_DECREMENT_OPERATOR";
        default: return "INVALID_TYPE";
    }
}
