#include <iostream>
#include "Compiler.h"

int main()
{
    Compiler compiler("/home/diego/Compiler/example/test.cmm", 0);
    int res = compiler.compile();
    try
    {
        std::cout << std::endl
                  << "Compilation successful." << std::endl;
    }
    catch (const std::exception &e)
    {
        // Catching the exception and retrieving the message
        std::cout << "Caught exception: " << e.what() << std::endl;
        std::cout << std::endl
                  << "Compilation failed." << std::endl;
    }

    return 0;
}
