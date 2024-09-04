#pragma once
#include "HandwrittenLexer.h"


HandwrittenLexer::HandwrittenLexer(std::shared_ptr<std::fstream> sourceCodeStream):sourceCodeStream(sourceCodeStream){

    keywords = {
         "if", "else", "while", "return", "auto", "break",
         "case", "const", "continue", "default", "do", "double",
         "enum", "extern", "float", "for", "int", "long",
         "short", "signed", "sizeof", "static", "struct",
         "switch", "typedef", "union", "unsigned", "void",
         "volatile", "char"
    };



}

HandwrittenLexer::~HandwrittenLexer() {

}

std::vector<Token> HandwrittenLexer::tokenize() {

    std::vector<Token> tokens;
    std::string currentToken;
    std::string code((std::istreambuf_iterator<char>(*sourceCodeStream)), std::istreambuf_iterator<char>());


    for (size_t i = 0; i < code.length(); ++i) {
        char c = code[i];

        // Skip whitespace
        if (std::isspace(c)) {
            continue;
        }

        // If the character is a letter, it's part of an identifier or keyword
        if (std::isalpha(c)) {
            currentToken.clear();
            while (i < code.length() && std::isalnum(code[i])) {
                currentToken += code[i++];
            }
            --i;
            tokens.push_back(Token(currentToken, classifyToken(currentToken)));
        }

        // If the character is a digit, it's part of a number
        else if (std::isdigit(c) || (c == '+' || c == '-') && i + 1 < code.length() && std::isdigit(code[i + 1])) {
            currentToken.clear();

            // If the character is a sign, add it to the currentToken
            if (c == '+' || c == '-') {
                currentToken += c;
                ++i;
            }

            // Continue reading digits
            while (i < code.length() && std::isdigit(code[i])) {
                currentToken += code[i++];
            }
            --i;
            tokens.push_back(Token(currentToken, TokenType::CONSTANT));
        }

        // If the character is an operator
        else if (isOperator(c)) {
            tokens.push_back(Token(std::string(1, c), TokenType::OPERATOR));
        }
        // Handle unknown characters
        else {
            tokens.push_back(Token(std::string(1, c), TokenType::UNKNOWN));
        }
    }

    return tokens;
}


bool HandwrittenLexer::isKeyword(const std::string& str) const {
    return keywords.find(str) != keywords.end();
}


bool HandwrittenLexer::isOperator(char c) const{
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '<':
    case '>':
    case '&':
    case '|':
    case '^':
    case '!':
    case '~':
    case '?':
    case ':':
    case '.':
    case ',':
        return true;
    default:
        return false;
    }
}


TokenType HandwrittenLexer::classifyToken(const std::string& str) const {
    if (isKeyword(str)) return TokenType::KEYWORD;
    if (std::isdigit(str[0])) return TokenType::CONSTANT;
    if (std::ispunct(str[0])) return TokenType::PUNCTUATION;
    //if (Token::isOperator(str[0])) return TokenType::OPERATOR;
    if (std::isalpha(str[0])) return TokenType::IDENTIFIER;
    return TokenType::UNKNOWN;
}


void HandwrittenLexer::printTokens(const std::vector<Token>& tokens) const {
    for (const Token& token : tokens) {
        std::cout << "Token: " << token.getValue() << ", Type: " << token.getTypeAsString() << std::endl;
    }
}