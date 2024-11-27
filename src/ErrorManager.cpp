#include "ErrorManager.h"

ErrorManager::ErrorManager(int line, const std::string &message)
    : line_(line), message_(message) {}


//SYNTACTIC EXCEPTIONS

SyntacticException::SyntacticException(int line, const std::string &message)
    : ErrorManager(line, message)
{
    this->fileReader = FileReader::getInstance();
}

const char *SyntacticException::what() const noexcept
{
    std::ostringstream oss;

    oss << "\033[34m"; // blue
    oss << " at line " << line_ << ": " << this->fileReader->getLine(line_);
    oss << "\033[31m"; // red
    oss << "\nSyntax Error: " << message_;
    oss << "\033[0m"; // normal

    error_message_ = oss.str();
    return error_message_.c_str();
}

//LEXER EXCEPTIONS

LexerException::LexerException(const std::string &file, int line, const std::string &message)
    : ErrorManager(line, message)
{
    this->fileReader = FileReader::getInstance();
}

const char *LexerException::what() const noexcept
{
    std::ostringstream oss;

    oss << "\033[34m"; // blue
    oss << " at line " << line_ << ": " << this->fileReader->getLine(line_);
    oss << "\033[31m"; // red
    oss << "\nLexer Error: " << message_;
    oss << "\033[0m"; // normal

    error_message_ = oss.str();
    return error_message_.c_str();
}

//SEMANTIC EXCEPTIONS

SemanticException::SemanticException(int line, const std::string &message)
    : ErrorManager(line, message)
{
    this->fileReader = FileReader::getInstance();
}

const char *SemanticException::what() const noexcept
{
    std::ostringstream oss;

    oss << "\033[34m"; // blue
    oss << " at line " << line_ << ": " << this->fileReader->getLine(line_);
    oss << "\033[31m"; // red
    oss << "\nSemantic Error: " << message_;
    oss << "\033[0m"; // normal

    error_message_ = oss.str();
    return error_message_.c_str();
}