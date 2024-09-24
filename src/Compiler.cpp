#include "Compiler.h"
#include "TableDrivenLexer.h"
#include "TokenTable.h"


std::shared_ptr<std::fstream> Compiler::openFilePointerUnique(const std::string& filename, std::ios::openmode mode) {
    auto file = std::make_shared<std::fstream>(filename, mode);
    if (!file->is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return nullptr;
    }
    return file;
}



Compiler::Compiler(const std::string& filename, const std::string& out_filename, int flags) : filename(filename), out_filename(out_filename)
{
    this->sourceCodeStream = openFilePointerUnique(filename, std::ios::in);
    this->outCodeStream = openFilePointerUnique(out_filename,  std::ios::out);
}


Compiler::~Compiler()
{
    if (sourceCodeStream && sourceCodeStream->is_open()) {
        sourceCodeStream->close();
    }

    if (outCodeStream && outCodeStream->is_open()) {
        outCodeStream->close();
    }
}

int Compiler::compile() {
    if(this->sourceCodeStream == nullptr || this->outCodeStream == nullptr){
        return 1;
    }

    //output to console the file to make sure it works

    TableDrivenLexer lexer(this->sourceCodeStream);
    lexer.tokenize();
    TokenTable tokens = lexer.getTokens();
    
    tokens.printTokens();
    //const std::vector<Token> &tokens = lexer.getTokens();


    return 0;
}
