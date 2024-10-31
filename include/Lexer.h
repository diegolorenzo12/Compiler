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

Lexer::Lexer(std::shared_ptr<std::fstream> sourceCodeStream):  sourceCodeStream(sourceCodeStream){}


void Lexer::tokenize(){
    Lexer lex(sourceCodeStream);

    yyFlexLexer lexer;                                                 // Create an instance of the Flex lexer
    lexer.switch_streams(sourceCodeStream.get()); // Switch input and output streams

    int tokenType;
    while ((tokenType = lexer.yylex()) != 0)
    {
        std::string tokenValue = lexer.YYText();
        Token token(tokenValue, static_cast<TokenType>(tokenType), 1);
        tokenTable.push_back(token);
    }
}


TokenTable Lexer::getTokens(){
    return this->tokenTable;
}
