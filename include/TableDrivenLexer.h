#pragma once
#include <cctype>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>


#define ASCII_SIZE 128
#define NUM_STATES 131

enum class State {
    Start,
    keyword,
    Identifier,
    Puntiation,
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

private:
    std::shared_ptr<std::fstream> sourceCodeStream;
    void addTransitionExcludingChars(int sourceState, int destinationState, const std::vector<char>& excludedChars);
    void addTransitionForSpecificChars(int sourceState, int destinationState, const std::vector<char>& specificChars);
    //void addTransitionForCharsWithFallback(int sourceState, int includedDestinationState, int fallbackDestinationState, const std::vector<char>& specificChars);
    //void addTransitionForAlfNumWithFallback(int sourceState, int includedDestinationState, int fallbackDestinationState, const std::vector<char>& specificChars);
    //void addTransitionForAlphaWithFallback(int sourceState, int includedDestinationState, int fallbackDestinationState, const std::vector<char>& specificChars);



	int transitionTable[NUM_STATES][ASCII_SIZE];
    StateType stateTypes[NUM_STATES];

    std::string getTokenTypeString(StateType stateType);


};