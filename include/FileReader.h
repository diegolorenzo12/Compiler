#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>

class FileReader
{
private:
    static std::unique_ptr<FileReader> instance;
    std::shared_ptr<std::fstream> fileStream;

    FileReader() = default;

public:
    // Delete copy constructor and assignment operator to enforce Singleton pattern
    FileReader(const FileReader &) = delete;
    FileReader &operator=(const FileReader &) = delete;

    // Static method to access the Singleton instance
    static FileReader *getInstance()
    {
        if (!instance)
        {
            instance = std::unique_ptr<FileReader>(new FileReader());
        }
        return instance.get();
    }

    bool openFile(const std::string &filename, std::ios_base::openmode mode);

    std::string getLine(int lineNumber);

    void closeFile();
    std::shared_ptr<std::fstream> getFileStream();
    ~FileReader();
};
