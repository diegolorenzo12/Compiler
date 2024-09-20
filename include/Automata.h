#include <vector>
#include <algorithm>
#include <iostream>

#define ASCII_SIZE 128
#define NUM_STATES 131

enum class StateType {
    Keyword,
    Identifier,
    Punctuation,
    StringLiteral,
    Constant,
    Operator,
    Comment,
    Error,
};

class Automata{
public:

    Automata();
    int makeTransition(int currentState, char input);
    StateType getStateType(int state);
    std::string getStateTypeString(StateType state);
    bool isErrorState(int state);

private:

    void create();
    void addTransitionExcludingChars(int sourceState, int destinationState, const std::vector<char>& excludedChars);
    void addTransitionForSpecificChars(int sourceState, int destinationState, const std::vector<char>& specificChars);

    int transitionTable[NUM_STATES][ASCII_SIZE];
    StateType stateTypes[NUM_STATES];
};