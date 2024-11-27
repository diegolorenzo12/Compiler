#include "Compiler.h"
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h"
#include "Lexer.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "PrintVisitor.h"

Compiler::Compiler(const std::string &filename, int flags)
    : filename(filename), flags(flags)
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
    if(flags == 1) {
        return 0;
    }
    //tokenTable.printTokens();

    Parser parser(tokenTable);
    std::unique_ptr<Program> ast = parser.parsePROGRAM();
    if(flags == 2) {
        return 0;
    }

    //PrintVisitor printVisitor;
    // ast->accept(printVisitor);

    auto globalScope = std::make_shared<SymbolTable>();
    SemanticAnalyzer semanticAnalyzer(globalScope);

    ast->accept(semanticAnalyzer);

    //std::cout << "\nGlobal Symbol Table:\n";
    //globalScope->printCurrentScope();

    return 0;
}
