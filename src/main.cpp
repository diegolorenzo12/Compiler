#include <iostream>
#include "Compiler.h"



int main() {
    Compiler compiler("./test.cmm", 0);
    int res = compiler.compile();
    if (res == 0) {
        std::cout << std::endl << "Compilation successful." << std::endl;
    } else {
        std::cout << std::endl << "Compilation failed." << std::endl;
    }
    return 0;
}
