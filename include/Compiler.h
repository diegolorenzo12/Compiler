#pragma once
#include <fstream>
#include <memory>
#include <iostream>
#include <iostream>
#include <string>

class Compiler
{
public:
    Compiler(const std::string& filename, const std::string& out_filename, int flags);
    ~Compiler();
    int compile();

private:
    std::shared_ptr<std::fstream> sourceCodeStream;
    std::shared_ptr<std::fstream> outCodeStream;
    const std::string filename;
    const std::string out_filename;
    std::string preprocessCode;


    std::shared_ptr<std::fstream> openFilePointerUnique(const std::string& filename, std::ios::openmode mode);
};
