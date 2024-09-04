#pragma once
#include <fstream>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include "Token.h"
#include <unordered_set>


class HandwrittenLexer {
public:
    HandwrittenLexer(std::shared_ptr<std::fstream> sourceCodeStream);
	~HandwrittenLexer();
    std::vector<Token> tokenize();
    void printTokens(const std::vector<Token>& tokens) const;


private:
	std::shared_ptr<std::fstream> sourceCodeStream;

    bool isOperator(char c) const;
    bool isKeyword(const std::string& str) const;


    TokenType classifyToken(const std::string& str) const;

    std::unordered_set<std::string> keywords;
};