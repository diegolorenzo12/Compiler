#include <gtest/gtest.h>
#include "Compiler.h"
#include "ErrorManager.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>


std::string createTempFile(const std::string& content) {
    std::string tempFileName = "temp_test_file_parser.cmm";
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

TEST(CompilerTests, MissingSemicolonError)
{
    std::string fileContent = R"(
    
    func int main() {
        x = 10 // Missing semicolon, syntax error

    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 2);
            compiler.compile(); }, SyntacticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, noClosingBrace)
{
    std::string fileContent = R"(
    
    func int main() {
        x = 10; // Missing semicolon, syntax error
        {
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 2);
            compiler.compile(); }, SyntacticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, CodeExample)
{
    std::string fileContent = R"(
    func int add(int a, int b) {
        int result =0+1;
        result += a;
        return 0;
    }

    struct myStruct {
        int x[10];
        int y;
    };

    func int main() {
    int x = 10, y = 20; //one line with multiple declaration
    int arr[2][2] = {
        {1,2},
        {1,2}
    };  //brace initializer
    
    char string[10] = "string";

    switch (x) {
        int newScope = 10;
        case 1:{
            x++;
            break;
        }
        case 2:
            break;
        default:
            {
                y++;
                newScope--;
                break;
            }
            
    }

    int n=10;

    for(int i=0; i<n; i++){
         i=10;
         break;
    }

    while(n>0){
        n--;
    }

    do{
        n++;
    }while(n<10);

    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_NO_THROW({
            Compiler compiler(tempFileName, 2);
            compiler.compile(); });

    deleteTempFile(tempFileName);
}