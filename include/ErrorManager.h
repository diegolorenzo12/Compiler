#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include "FileReader.h"

class ErrorManager : public std::exception
{
public:
    ErrorManager(int line, const std::string &message);
    
    virtual ~ErrorManager() = default;

protected:
    FileReader *fileReader = nullptr;
    int line_;
    std::string message_;
    mutable std::string error_message_;
};

class LexerException : public ErrorManager
{
public:
    LexerException(const std::string &file, int line, const std::string &message);

    const char *what() const noexcept override;
};

class SyntacticException : public ErrorManager
{
public:
    SyntacticException(int line, const std::string &message);

    const char *what() const noexcept override;
};

class SemanticException : public ErrorManager
{
public:
    SemanticException(int line, const std::string &message);

    const char *what() const noexcept override;
};
