%{
#ifdef _WIN32
    /* Windows doesn't have unistd.h */
#else
    #include <unistd.h>
#endif
#include <iostream>
#include <string>
#include "Token.h"      

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
"auto"          { return (int)TokenType::KEYWORD; }
"break"         { return (int)TokenType::KEYWORD; }
"case"          { return (int)TokenType::KEYWORD; }

{number}        { return (int)TokenType::CONSTANT; }

{string}        { return (int)TokenType::STRING_LITERAL; }

{punctuation}   { return (int)TokenType::PUNCTUATION; }

{arith_op}      { return (int)TokenType::ARITHMETIC_OPERATOR; }

{logical_op}    { return (int)TokenType::LOGICAL_OPERATOR; }

{comp_op}       { return (int)TokenType::COMPARISON_OPERATOR; }

{assign_op}     { return (int)TokenType::ASSIGNMENT_OPERATOR; }

{increment_op}  { return (int)TokenType::INCREMENT_DECREMENT_OPERATOR; }

{identifier}    { return (int)TokenType::IDENTIFIER; }

[ \t\n\r]+      /* Ignore whitespace */

.               { return (int)TokenType::UNKNOWN; }

%%
