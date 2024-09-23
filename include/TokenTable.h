#pragma once
#include <string>
#include <deque>
#include "Token.h"
#include <iostream>

class TokenTable{
public:
    void push_back(Token t);
    bool isEmpty();
    void clear();
    void printTokens() const;


private:
    std::deque<Token> tokens;
};

