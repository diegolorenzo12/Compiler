#include "FileReader.h"

std::unique_ptr<FileReader> FileReader::instance = nullptr;

// Method to open a file for reading or writing
bool FileReader::openFile(const std::string &filename, std::ios_base::openmode mode)
{
    if (fileStream && fileStream->is_open())
    {
        throw std::runtime_error("A file is already open!");
    }

    fileStream = std::make_shared<std::fstream>(filename, mode);

    if (!fileStream->is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return true;
}

// Method to get a specific line from the file
std::string FileReader::getLine(int lineNumber)
{
    if (!fileStream || !fileStream->is_open())
    {
        throw std::runtime_error("File not opened.");
    }

    fileStream->clear();
    fileStream->seekg(0, std::ios::beg);

    std::string line;
    int currentLine = 1;

    while (std::getline(*fileStream, line))
    {
        if (currentLine == lineNumber)
        {
            return line;
        }
        ++currentLine;
    }

    throw std::out_of_range("Line number out of range.");
}

// Method to close the file
void FileReader::closeFile()
{
    if (fileStream && fileStream->is_open())
    {
        fileStream->close();
    }
}

// Method to get the shared pointer to the file stream
std::shared_ptr<std::fstream> FileReader::getFileStream()
{
    if (!fileStream || !fileStream->is_open())
    {
        throw std::runtime_error("File not opened.");
    }
    return fileStream;
}

// Destructor to ensure the file is closed when the instance is destroyed
FileReader::~FileReader()
{
    closeFile();
}
