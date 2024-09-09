#pragma once
#include <string>

enum class TokenType
{
    KEYWORD,
    IDENTIFIER,
    PUNCTUATION,
    CONSTANT,
    OPERATOR,
    STRING_LITERAL,
    COMMENT,
    UNKNOWN
};

class Token {
public:
    Token(const std::string& value, TokenType type) : value(value), type(type) {}

    std::string getValue() const;
    TokenType getType() const;
    std::string getTypeAsString() const;

private:
    std::string value;
    TokenType type;
};