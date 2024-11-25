#include "Compiler.h"
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h"
#include "Lexer.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "PrintVisitor.h"

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
    // tokenTable.printTokens();

    Parser parser(tokenTable);
    std::unique_ptr<Program> ast = parser.parsePROGRAM();

    PrintVisitor printVisitor;

    // ast->accept(printVisitor);

    auto globalScope = std::make_shared<SymbolTable>();
    SemanticAnalyzer semanticAnalyzer(globalScope);

    ast->accept(semanticAnalyzer);

    std::cout << "\nSymbol Table:\n";
    globalScope->printCurrentScope();

    return 0;
}
