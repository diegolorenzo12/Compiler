#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include "FileReader.h"

class LexerException : public std::exception
{
public:
    LexerException(const std::string &file, int line, const std::string &message);

    const char *what() const noexcept override;

private:
    FileReader *fileReader;
    int line_;
    std::string message_;
    mutable std::string error_message_;
};