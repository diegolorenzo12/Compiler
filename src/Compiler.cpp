#include "Compiler.h"
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h" 
#include "Lexer.h"


std::shared_ptr<std::fstream> Compiler::openFilePointerUnique(const std::string& filename, std::ios::openmode mode) {
    auto file = std::make_shared<std::fstream>(filename, mode);
    if (!file->is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return nullptr;
    }
    return file;
}



Compiler::Compiler(const std::string& filename, int flags) 
    : filename(filename) {
    this->sourceCodeStream = openFilePointerUnique(filename, std::ios::in);
}


Compiler::~Compiler()
{
    if (sourceCodeStream && sourceCodeStream->is_open()) {
        sourceCodeStream->close();
    }
}

int Compiler::compile() {
    if (this->sourceCodeStream == nullptr) {
        return 1;
    }

    Lexer lex(sourceCodeStream);
    lex.tokenize();
    TokenTable tokenTable = lex.getTokens();
    tokenTable.printTokens();

    //pass the token table to the parser

    return 0;
}
