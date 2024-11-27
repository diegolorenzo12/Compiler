#include <gtest/gtest.h>
#include "Compiler.h"
#include "ErrorManager.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>


std::string createTempFile(const std::string& content) {
    std::string tempFileName = "temp_test_file_semantic.cmm";
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

TEST(CompilerTests, noDeclared)
{
    std::string fileContent = R"(
    
    func int main() {
        x = 10; //not declared
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, outOfScope)
{
    std::string fileContent = R"(
    
    func int main() {
       { 
            int x = 10; 
       }
         x = 10; //out of scope
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, functionSignatureMatch)
{
    std::string fileContent = R"(


    func int add(int a[], int b) {
        int result =0+1;
        result += a; //not match type array
        return 0;
    }

    func int main() {
       { 
            int x = 10; 
       }
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, ReturnTypeMatch)
{
    std::string fileContent = R"(


    func int add(int a[], int b) {
        int result =0+1;
        result += a[0]; //not match type array
        return "This is a char array";
    }

    func int main() {
       { 
            int x = 10; 
       }
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, FunctionSignatureWithFunctionCall)
{
    std::string fileContent = R"(


    func int add(int a, int b) {
        int result =0+1;
        result += a; //not match type array
        return result;
    }

    func int main() {
       { 
            int x = 10; 
       }

        add(1,2,3); //too many arguments
        add("asfsadf",3);
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, StructMemberTypes)
{
    std::string fileContent = R"(


    func int add(int a, int b) {
        int result =0+1;
        result += a; //not match type array
        return result;
    }

    struct myStruct {
        int x[10];
        int y;
    };

    func int main() {
       { 
            int x = 10; 
       }

       struct myStruct s;

       s.x = 10; //not match type array

        add(1,2);
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, StructMemberDontExist)
{
    std::string fileContent = R"(


    func int add(int a, int b) {
        int result =0+1;
        result += a; //not match type array
        return result;
    }

    struct myStruct {
        int x[10];
        int y;
    };

    func int main() {
       { 
            int x = 10; 
       }

       struct myStruct s;

       s.w = 10; //member not in MyStruct
        add(1,2);
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, ArrowOperator)
{
    std::string fileContent = R"(


    func int add(int a, int b) {
        int result =0+1;
        result += a; //not match type array
        return result;
    }

    struct myStruct {
        int x[10];
        int y;
    };

    func int main() {
       { 
            int x = 10; 
       }

       struct myStruct *s;

       s->x[1]=10;
       s.y=0; //not match type, s is a pointer, must use ->
        add(1,2);
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, BraceInitializerDimension)
{
    std::string fileContent = R"(


    func int main() {

       int arr[2][2] = {
        {1,2},
        {1,2}
        };  //brace initializer correct 
    
        char string[10] = "string"; //correct 


        int newarr[2][2] = {
        {
            {1, 2},  
            {3, 4}   
        },
        {
            {5, 6},  
            {7, 8}   
        }
         };   //dimensions dont match


    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, BreakStatementAndContinueStatement)
{
    std::string fileContent = R"(


    func int main() {

       {
            break;
            continue;
        }  //break and continue not in loop or switch
    })";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, CaseOursideSwitch)
{
    std::string fileContent = R"(

    func int main() {
           case (1):{
            break;
           }
    }
    )";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, CaseNotConstant)
{
    std::string fileContent = R"(

    func int main() {
        int x = 10;

        switch (10) {
            case (x):{
            break;
           }
        }
    }
    )";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); }, SemanticException);

    deleteTempFile(tempFileName);
}

TEST(CompilerTests, CorrectCodeExample)
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



func float prueba(){   
    struct myStruct * point;
    point->x[1] = 1;
    point->y = 5;

    struct myStruct point2;
    point2.x[1] = 1;
    point2.y = 5;
    {
        int point; //on another scope 
        point = 1;
        return 0.0;
    }

    struct anonimousStruct{
        int point[10];
        int test;
    };
    anonimousStruct.test = 2;
}



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

    int result = 1+2;

    {

    }


    // Add two numbers
    result = add(x, y);
    
    // Conditional statement
    if (result=1) {
    }

    int n=10;

    for(int i=0; n; i++){
        i=10;
    }

    do{
    }while(1);

    while(1){
        continue;
        break;
    }
    return 0;
}

    )";
    std::string tempFileName = createTempFile(fileContent);

    EXPECT_NO_THROW({
            Compiler compiler(tempFileName, 0);
            compiler.compile(); });

    deleteTempFile(tempFileName);
}