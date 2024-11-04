#include <iostream>
#include "Compiler.h"

int main()
{
    Compiler compiler("./test.cmm", 0);
    try
    {
        int res = compiler.compile();
        // Throwing an exception with a custom message
        if (res == 0)
        {
            std::cout << std::endl
                      << "Compilation successful." << std::endl;
        }
        else
        {
            std::cout << std::endl
                      << "Compilation failed." << std::endl;
        }
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
