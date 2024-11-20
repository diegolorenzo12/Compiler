#include "Lexer.h"
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h"

Lexer::Lexer(std::shared_ptr<std::fstream> sourceCodeStream) : sourceCodeStream(sourceCodeStream) {}

void Lexer::tokenize()
{
    Lexer lex(sourceCodeStream);

    yyFlexLexer lexer;
    lexer.switch_streams(sourceCodeStream.get()); // Switch input and output streams

    int tokenType;
    while ((tokenType = lexer.yylex()) != 0)
    {
        std::string tokenValue = lexer.YYText();
        Token token(tokenValue, static_cast<TokenType>(tokenType), lexer.lineno());
        tokenTable.push_back(token);
    }
}

TokenTable &Lexer::getTokens()
{
    return tokenTable; // Return by reference
}
