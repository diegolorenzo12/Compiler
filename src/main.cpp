#include <iostream>
#include "Compiler.h"



int main() {
    Compiler compiler("../example/test.cmm", "../example/test", 0);

    int res = compiler.compile();
    if (res == 0)
    {
        std::cout << std::endl << std::endl << "everything compiled file\n";
    }
    else if (res == 1)
    {
        std::cout<<std::endl<<std::endl<<"Compile failed\n";
    }
    else
    {
        std::cout << std::endl << "Unknown response for compile time\n";
    }
    return 0;
}