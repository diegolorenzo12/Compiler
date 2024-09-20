#pragma once
#include <cctype>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Token.h"
#include "Automata.h"

#define ASCII_SIZE 128
#define NUM_STATES 131

class TableDrivenLexer {
public:
    TableDrivenLexer(std::shared_ptr<std::fstream> sourceCodeStream);
    void tokenize();
    void printTokens() const;
    const std::vector<Token> &getTokens() const;

    std::vector<Token> tokens;

private:
    std::shared_ptr<std::fstream> sourceCodeStream;
    TokenType StateTypeToTokenType(StateType stateType);


    Automata automata;
    std::string getTokenTypeString(StateType stateType);
};