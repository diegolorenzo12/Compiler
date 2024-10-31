#pragma once
#include <string>

enum class TokenType
{
    UNKNOWN,
    IDENTIFIER,
    KEYWORD,
    PUNCTUATION,
    INTEGER_CONSTANT,
    FLOAT_CONSTANT,
    OPERATOR,
    STRING_LITERAL,
    ARITHMETIC_OPERATOR,
    LOGICAL_OPERATOR,
    COMPARISON_OPERATOR,
    ASSIGNMENT_OPERATOR,
    INCREMENT_OPERATOR,
    END_OF_FILE
};

class Token
{
public:
    // Token(const std::string& value, TokenType type) : value(value), type(type) {}
    Token() : type(TokenType::UNKNOWN), value(""), lineNumber(0) {}

    Token(const std::string &value, TokenType type, int lineNumber)
        : value(value), type(type), lineNumber(lineNumber) {}

    std::string getValue() const;
    TokenType getType() const;
    std::string getTypeAsString() const;
    int getLineNumber() const { return lineNumber; }

private:
    std::string value;
    TokenType type;
    int lineNumber;
};
