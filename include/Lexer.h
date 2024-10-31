#pragma once
#include <fstream>
#include <memory>
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h" 

class Lexer{

public:
    Lexer(std::shared_ptr<std::fstream> sourceCodeStream);
    void tokenize();
    TokenTable getTokens();

private:
    TokenTable tokenTable;
    std::shared_ptr<std::fstream> sourceCodeStream;
};
