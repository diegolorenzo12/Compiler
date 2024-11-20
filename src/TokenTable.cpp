#include "TokenTable.h"

void TokenTable::push_back(Token t)
{
    tokens.push_back(t);
}

void TokenTable::pop_front()
{
    tokens.pop_front();
}

Token TokenTable::front()
{
    if (isEmpty())
    {
        return Token("", TokenType::END_OF_FILE, 0);
    }
    return tokens.front();
}

bool TokenTable::isEmpty()
{
    return tokens.empty();
}

int TokenTable::size()
{
    return tokens.size();
}

void TokenTable::clear()
{
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
        std::cout << "Token: " << token.getValue() << ", Type: " << token.getTypeAsString() << ", LineNo:" << token.getLineNumber() << std::endl;
    }
}