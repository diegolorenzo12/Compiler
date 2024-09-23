#pragma once
#include <cctype>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Token.h"
#include "Automata.h"
#include "TokenBuffer.h"
#include "TokenTable.h"



#define ASCII_SIZE 128
#define NUM_STATES 131

class TableDrivenLexer {
public:
    TableDrivenLexer(std::shared_ptr<std::fstream> sourceCodeStream);
    void tokenize();
    //void printTokens() const;
    const TokenTable &getTokens() const;

private:
    std::shared_ptr<std::fstream> sourceCodeStream;
    TokenType StateTypeToTokenType(StateType stateType);


    Automata automata;
    std::string getTokenTypeString(StateType stateType);
    TokenTable tokens;
};