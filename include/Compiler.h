#pragma once
#include <fstream>
#include <memory>
#include <iostream>
#include <iostream>
#include <string>

class Compiler
{
public:
    Compiler(const std::string& filename, int flags);
    ~Compiler();
    int compile();

private:
    std::shared_ptr<std::fstream> sourceCodeStream;
    const std::string filename;
    std::string preprocessCode;


    std::shared_ptr<std::fstream> openFilePointerUnique(const std::string& filename, std::ios::openmode mode);
};
