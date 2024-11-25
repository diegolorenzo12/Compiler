#include <iostream>
#include "Compiler.h"

int main()
{
    Compiler compiler("./test.cmm", 0);
    try
    {
        int res = compiler.compile();
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
