#include "TokenBuffer.h"

//TokenBuffer::TokenBuffer(){}

void TokenBuffer::add(char c){
    buffer.push_back(c);
}

std::string TokenBuffer::getString(){
    return std::string(buffer.begin(), buffer.end());
}

void TokenBuffer::clear(){
    buffer.clear();
}

bool TokenBuffer::isEmpty(){
    return buffer.empty();
}