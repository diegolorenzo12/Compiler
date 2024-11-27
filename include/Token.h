#pragma once
#include <string>

enum class TokenType
{
    UNKNOWN,
    IDENTIFIER,
    INLINE,
    RESTRICT,
    VOLATILE,
    STATIC,
    AUTO,
    REGISTER,
    CONST,
    CHAR,
    SHORT,
    LONG,
    FLOAT,
    DOUBLE,
    SIGNED,
    UNSIGNED,
    BOOL,
    STRUCT,
    IF,
    ELSE,
    SWITCH,
    CASE,
    DEFAULT,
    WHILE,
    DO,
    FOR,
    CONTINUE,
    BREAK,
    RETURN,
    SIZEOF,
    VOID,
    INT,
    FUNC,
    RIGHT_ASSIGN,
    LEFT_ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    AND_ASSIGN,
    XOR_ASSIGN,
    OR_ASSIGN,
    RIGHT_OP,
    LEFT_OP,
    INC_OP,
    DEC_OP,
    PTR_OP,
    AND_OP,
    OR_OP,
    LE_OP,
    GE_OP,
    EQ_OP,
    NE_OP,
    SEMICOLON,
    LBRACE,
    RBRACE,
    COMMA,
    COLON,
    EQUAL,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    DOT,
    AMPERSAND,
    EXCLAMATION,
    TILDE,
    MINUS,
    PLUS,
    ASTERISK,
    SLASH,
    PERCENT,
    LESS_THAN,
    GREATER_THAN,
    CARET,
    PIPE,
    QUESTION_MARK,
    INTEGER_CONSTANT,
    FLOAT_CONSTANT,
    STRING_LITERAL,
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
