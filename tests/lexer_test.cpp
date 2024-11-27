#include <gtest/gtest.h>
#include "Compiler.h"
#include "ErrorManager.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>


std::string createTempFile(const std::string& content) {
    std::string tempFileName = "temp_test_file_lexer.cmm";
    std::ofstream tempFile(tempFileName);
    if (!tempFile) {
        throw std::runtime_error("Unable to create temporary file");
    }
    tempFile << content;
    tempFile.close();
    return tempFileName;
}

void deleteTempFile(const std::string &fileName)
{
    if (std::remove(fileName.c_str()) != 0)
    {
        std::cerr << "Failed to delete temporary file: " << fileName << std::endl;
    }
}

TEST(CompilerTests, unregonizedTokenError)
{
    std::string fileContent = R"(
    
    int main() {
        $ // unregonized token, lexer error
        x = 1;

    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 1);
            compiler.compile(); }, LexerException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, testAllTokenTypes)
{
    const std::string testInput = R"(
        inline restrict volatile static auto register const char short long float double signed unsigned bool struct
        if else switch case default while do for continue break return sizeof void int func
        >>= <<= += -= *= /= %= &= ^= |= >> << ++ -- -> || && <= >= == != { ; } , : = ( ) [ ] . & ! ~ - + * / % < > ^ | ?
        // This is a single-line comment
        /* This is a
        multi-line comment */
        "string_literal_example"
        12345
        67890.12345
        identifier_example)";
    std::string tempFileName = createTempFile(testInput);

    EXPECT_NO_THROW({
            Compiler compiler(tempFileName, 1);
            compiler.compile(); });

    deleteTempFile(tempFileName);
}
