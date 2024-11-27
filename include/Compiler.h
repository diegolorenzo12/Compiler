#pragma once
#include <fstream>
#include <memory>
#include <iostream>
#include <iostream>
#include <string>
#include "Parser.h"
#include "FileReader.h"

class Compiler
{
public:
    Compiler(const std::string &filename, int flags);
    ~Compiler();
    int compile();

private:
    const std::string filename;
    FileReader *fileReader;
    int flags;
};
