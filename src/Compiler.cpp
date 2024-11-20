#include "Compiler.h"
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h"
#include "Lexer.h"

Compiler::Compiler(const std::string &filename, int flags)
    : filename(filename)
{
    this->fileReader = FileReader::getInstance();
    this->fileReader->openFile(filename, std::ios::in);
}

Compiler::~Compiler()
{
    this->fileReader->closeFile();
}

int Compiler::compile()
{
    Lexer lex(fileReader->getFileStream());
    lex.tokenize();
    TokenTable &tokenTable = lex.getTokens();
    tokenTable.printTokens();

    Parser parser(tokenTable);
    parser.parsePROGRAM();

    return 0;
}
