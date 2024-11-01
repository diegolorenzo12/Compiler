%{
#ifdef _WIN32
    /* Windows doesn't have unistd.h */
#else
    #include <unistd.h>
#endif
#include <string>
#include "Token.h"   
extern void yyerror(const char *);  

%}

%option noyywrap c++ 

%x COMMENT 

identifier      [A-Za-z_][A-Za-z0-9_]*
int_constant    [0-9]+ 
float_constant  [0-9]+\.[0-9]+([eE][-+]?[0-9]+)? 
string          \"[^\n"]*\"
punctuation     [;,:.()->{}[\]?]
comp_op        (==|!=|<=|>=|<|>|&&|\|\|)
increment_op    (\+\+|\-\-)
assign_op      (=|\+=|\-=|\*=|\/=|%=|<<=|>>=|&=|\^=|\|=)
arithmetic_op  [-+*/%&|^]


%%

"inline"    { return (int)TokenType::KEYWORD; }

"restrict"  { return (int)TokenType::KEYWORD; }

"volatile"  { return (int)TokenType::KEYWORD; }

"static"    { return (int)TokenType::KEYWORD; }

"auto"      { return (int)TokenType::KEYWORD; }

"register"  { return (int)TokenType::KEYWORD; }

"const"     { return (int)TokenType::KEYWORD; }

"char"      { return (int)TokenType::KEYWORD; }

"short"     { return (int)TokenType::KEYWORD; }

"long"      { return (int)TokenType::KEYWORD; }

"float"     { return (int)TokenType::KEYWORD; }

"double"    { return (int)TokenType::KEYWORD; }

"signed"    { return (int)TokenType::KEYWORD; }

"unsigned"  { return (int)TokenType::KEYWORD; }

"bool"      { return (int)TokenType::KEYWORD; }

"struct"    { return (int)TokenType::KEYWORD; }

"if"        { return (int)TokenType::KEYWORD; }

"else"      { return (int)TokenType::KEYWORD; }

"switch"    { return (int)TokenType::KEYWORD; }

"case"      { return (int)TokenType::KEYWORD; }

"default"   { return (int)TokenType::KEYWORD; }

"while"     { return (int)TokenType::KEYWORD; }

"do"        { return (int)TokenType::KEYWORD; }

"for"       { return (int)TokenType::KEYWORD; }

"continue"  { return (int)TokenType::KEYWORD; }

"break"     { return (int)TokenType::KEYWORD; }

"return"    { return (int)TokenType::KEYWORD; }

"sizeof"    { return (int)TokenType::KEYWORD; }

"void"      { return (int)TokenType::KEYWORD; }

"int"       { return (int)TokenType::KEYWORD; }

"func"       { return (int)TokenType::KEYWORD; }


{string}        { return (int)TokenType::STRING_LITERAL; }

{int_constant}        { return (int)TokenType::INTEGER_CONSTANT; }

{float_constant}      {return (int)TokenType::FLOAT_CONSTANT;}

{string}              { return (int)TokenType::STRING_LITERAL; }

{assign_op}       { return (int)TokenType::ASSIGNMENT_OPERATOR; }

{punctuation}   { return (int)TokenType::PUNCTUATION; }

{comp_op}      { return (int)TokenType::COMPARISON_OPERATOR; }

{increment_op}  { return (int)TokenType::INCREMENT_OPERATOR; }


{arithmetic_op}  { return (int)TokenType::ARITHMETIC_OPERATOR; }

{identifier}    { return (int)TokenType::IDENTIFIER; }

[ \t\n\r]+      /* Ignore whitespace */

.               {fprintf(stderr, "Error: Unrecognized character '%s'\n", yytext);  return (int)TokenType::UNKNOWN; }

"//".*          { /* Ignore single-line comments */ }

"/*"            { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n   { }
<COMMENT>.    { }

%%