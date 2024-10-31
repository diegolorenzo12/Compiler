#include "Compiler.h"
#include "FlexLexer.h"
#include "TokenTable.h"
#include "Token.h" 


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

    yyFlexLexer lexer;                                                 // Create an instance of the Flex lexer
    lexer.switch_streams(sourceCodeStream.get()); // Switch input and output streams

    TokenTable tokenTable;

    // Start lexing and store tokens in the TokenTable
    int tokenType;
    std::cout<<lexer.yylex()<<": token"<<std::endl;
    while ((tokenType = lexer.yylex()) != 0)
    {
        std::string tokenValue = lexer.YYText();
        Token token(tokenValue, static_cast<TokenType>(tokenType), 1);
        tokenTable.push_back(token);
    }

    // Optionally, print the tokens for debugging
    tokenTable.printTokens();

    return 0;
}
