#include "TokenTable.h"


void TokenTable::push_back(Token t){
    tokens.push_back(t);
}
//std::string getTokens();
bool TokenTable::isEmpty(){
    return tokens.empty();
}

void TokenTable::clear(){
    tokens.clear();
}

void TokenTable::printTokens() const
{
    if (tokens.empty())
    {
        std::cout << "No tokens to print." << std::endl;
        return;
    }

    for (const Token &token : tokens)
    {
        std::cout << "Token: " << token.getValue() << ", Type: " << token.getTypeAsString() << std::endl;
    }
}