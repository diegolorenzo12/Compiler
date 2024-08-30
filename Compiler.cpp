#include "Compiler.h"



std::unique_ptr<std::fstream> Compiler::openFilePointerUnique(const std::string& filename, std::ios::openmode mode) {
    auto file = std::make_unique<std::fstream>(filename, mode);
    if (!file->is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return nullptr;
    }
    return file;
}



Compiler::Compiler(const std::string& filename, const std::string& out_filename, int flags) : filename(filename), out_filename(out_filename)
{
    this->sourceCode = openFilePointerUnique(filename, std::ios::in);
    this->outCode = openFilePointerUnique(out_filename,  std::ios::out);
}

Compiler::~Compiler()
{
}


int Compiler::compile() {
    if(this->sourceCode == nullptr || this->outCode == nullptr){
        return 1;
    }

    //output to console the file to make sure it works
    std::string line;
    while (std::getline(*sourceCode, line)) {
        std::cout << line << std::endl;
    }

    
    return 0;
}