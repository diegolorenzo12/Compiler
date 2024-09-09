#pragma once
#include <cctype>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Token.h"

#define ASCII_SIZE 128
#define NUM_STATES 131

enum class State
{
    Start,
    keyword,
    Identifier,
    Punctuation,
    Constant,
    Operator,
    Error,
};

enum class StateType {
    Start,
    Keyword,
    Identifier,
    Punctuation,
    StringLiteral,
    Constant,
    Operator,
    Comment,
    Error,
};



class TableDrivenLexer {
public:
    TableDrivenLexer(std::shared_ptr<std::fstream> sourceCodeStream);
    void tokenize();
    void printTokens() const;
    const std::vector<Token> &getTokens() const;

    std::vector<Token> tokens;

private:
    std::shared_ptr<std::fstream> sourceCodeStream;
    void addTransitionExcludingChars(int sourceState, int destinationState, const std::vector<char>& excludedChars);
    void addTransitionForSpecificChars(int sourceState, int destinationState, const std::vector<char>& specificChars);
    TokenType StateTypeToTokenType(StateType stateType);


    int transitionTable[NUM_STATES][ASCII_SIZE];
    StateType stateTypes[NUM_STATES];

    std::string getTokenTypeString(StateType stateType);

};