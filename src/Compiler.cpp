#include "Compiler.h"
#include "FlexLexer.h" // Include the generated Flex header
#include "TokenTable.h"
#include "Token.h" // Include your Token class header


std::shared_ptr<std::fstream> Compiler::openFilePointerUnique(const std::string& filename, std::ios::openmode mode) {
    auto file = std::make_shared<std::fstream>(filename, mode);
    if (!file->is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return nullptr;
    }
    return file;
}



Compiler::Compiler(const std::string& filename, const std::string& out_filename, int flags) 
    : filename(filename), out_filename(out_filename) {
    this->sourceCodeStream = openFilePointerUnique(filename, std::ios::in);
    this->outCodeStream = openFilePointerUnique(out_filename, std::ios::out);
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
    if (this->sourceCodeStream == nullptr || this->outCodeStream == nullptr) {
        return 1;
    }

    yyFlexLexer lexer; // Create an instance of the Flex lexer
    lexer.switch_streams(sourceCodeStream.get(), outCodeStream.get()); // Switch input and output streams

    // Start lexing
    while (lexer.yylex() != 0) {
        // Continue lexing until EOF
    }

    // Retrieve the token table from the lexer
    TokenTable tokenTable = getTokenTable(); // Call the function to get the token table

    // Optionally, print the tokens for debugging
    tokenTable.printTokens();

    return 0;
}
