%{
#include <iostream>
#include <string>
#include "TokenTable.h"
using namespace std;

TokenTable tokenTable; // Declare a global TokenTable instance
%}

%option noyywrap c++

/* Define regular expressions for different tokens */
identifier      [A-Za-z_][A-Za-z0-9_]*
number          [0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
string          \"[^\n"]*\"
punctuation     [;,\.\(\)\[\]\{\}]
arith_op        [+\-*/%]
logical_op      (\|\||&&|!)
comp_op         (==|!=|<=|>=|<|>)
assign_op       (=|\+=|\-=|\*=|\/=|%=)
increment_op    (\+\+|\-\-)

%%
"auto"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"break"         { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"case"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"char"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"const"         { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"continue"      { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"do"            { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"double"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"else"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"enum"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"extern"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"float"         { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"for"           { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"goto"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"if"            { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"int"           { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"long"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"register"      { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"return"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"short"         { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"signed"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"sizeof"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"static"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"struct"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"switch"        { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"typedef"       { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"union"         { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"unsigned"      { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"void"          { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"volatile"      { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }
"while"         { tokenTable.push_back(Token(YYText(), TokenType::KEYWORD, lineNumber)); }

{number}        { tokenTable.push_back(Token(YYText(), TokenType::CONSTANT, lineNumber)); }

{string}        { tokenTable.push_back(Token(YYText(), TokenType::STRING_LITERAL, lineNumber)); }

{punctuation}   { tokenTable.push_back(Token(YYText(), TokenType::PUNCTUATION, lineNumber)); }

{arith_op}      { tokenTable.push_back(Token(YYText(), TokenType::ARITHMETIC_OPERATOR, lineNumber)); }

{logical_op}    { tokenTable.push_back(Token(YYText(), TokenType::LOGICAL_OPERATOR, lineNumber)); }

{comp_op}       { tokenTable.push_back(Token(YYText(), TokenType::COMPARISON_OPERATOR, lineNumber)); }

{assign_op}     { tokenTable.push_back(Token(YYText(), TokenType::ASSIGNMENT_OPERATOR, lineNumber)); }

{increment_op}  { tokenTable.push_back(Token(YYText(), TokenType::INCREMENT_DECREMENT_OPERATOR, lineNumber)); }

{identifier}    { tokenTable.push_back(Token(YYText(), TokenType::IDENTIFIER, lineNumber)); }

[ \t\n\r]+      /* Ignore whitespace */

.               { tokenTable.push_back(Token(YYText(), TokenType::UNKNOWN, lineNumber)); }

%%

// Add a function to return the token table
TokenTable getTokenTable() {
    return tokenTable;
}

