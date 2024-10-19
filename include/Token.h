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
    UNKNOWN,
    ARITHMETIC_OPERATOR, 
    LOGICAL_OPERATOR,    
    COMPARISON_OPERATOR, 
    ASSIGNMENT_OPERATOR, 
    INCREMENT_DECREMENT_OPERATOR 
};

class Token {
public:
    // Token(const std::string& value, TokenType type) : value(value), type(type) {}
    Token(const std::string &value, TokenType type, int lineNumber)
        : value(value), type(type), lineNumber(lineNumber) {}

    std::string getValue() const;
    TokenType getType() const;
    std::string getTypeAsString() const;
    int getLineNumber() const { return lineNumber; }

private:
    std::string value;
    TokenType type;
    //int lineNumber;
    int lineNumber;
};
