#include <vector>
#include <string>

class TokenBuffer{
public:
    //TokenBuffer();
    void add(char c);
    std::string getString();
    bool isEmpty();
    void clear();


private:
    std::vector<char> buffer;
};