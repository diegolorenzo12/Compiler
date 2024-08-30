#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class Compiler
{
public:
    Compiler(const std::string& filename, const std::string& out_filename, int flags);
    ~Compiler();
    int compile();

private:
    std::unique_ptr<std::fstream> sourceCode;
    std::unique_ptr<std::fstream> outCode;
    const std::string filename;
    const std::string out_filename;


    std::unique_ptr<std::fstream> openFilePointerUnique(const std::string& filename, std::ios::openmode mode);
};
