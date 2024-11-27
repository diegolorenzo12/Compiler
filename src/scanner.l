%{
#ifdef _WIN32
    /* Windows doesn't have unistd.h */
#else
    #include <unistd.h>
#endif
#include <string>
#include "Token.h"   
#include "ErrorManager.h"
extern void yyerror(const char *);  

%}

%option noyywrap c++ 
%option yylineno



%x COMMENT 

identifier      [A-Za-z_][A-Za-z0-9_]*
int_constant    [0-9]+ 
float_constant  [0-9]+\.[0-9]+([eE][-+]?[0-9]+)? 
string          \"[^\n"]*\"


%%

"inline"    { return (int)TokenType::INLINE; }

"restrict"  { return (int)TokenType::RESTRICT; }

"volatile"  { return (int)TokenType::VOLATILE; }

"static"    { return (int)TokenType::STATIC; }

"auto"      { return (int)TokenType::AUTO; }

"register"  { return (int)TokenType::REGISTER; }

"const"     { return (int)TokenType::CONST; }

"char"      { return (int)TokenType::CHAR; }

"short"     { return (int)TokenType::SHORT; }

"long"      { return (int)TokenType::LONG; }

"float"     { return (int)TokenType::FLOAT; }

"double"    { return (int)TokenType::DOUBLE; }

"signed"    { return (int)TokenType::SIGNED; }

"unsigned"  { return (int)TokenType::UNSIGNED; }

"bool"      { return (int)TokenType::BOOL; }

"struct"    { return (int)TokenType::STRUCT; }

"if"        { return (int)TokenType::IF; }

"else"      { return (int)TokenType::ELSE; }

"switch"    { return (int)TokenType::SWITCH; }

"case"      { return (int)TokenType::CASE; }

"default"   { return (int)TokenType::DEFAULT; }

"while"     { return (int)TokenType::WHILE; }

"do"        { return (int)TokenType::DO; }

"for"       { return (int)TokenType::FOR; }

"continue"  { return (int)TokenType::CONTINUE; }

"break"     { return (int)TokenType::BREAK; }

"return"    { return (int)TokenType::RETURN; }

"sizeof"    { return (int)TokenType::SIZEOF; }

"void"      { return (int)TokenType::VOID; }

"int"       { return (int)TokenType::INT; }

"func"       { return (int)TokenType::FUNC; }


">>="		{ return (int)TokenType::RIGHT_ASSIGN; }
"<<="		{ return (int)TokenType::LEFT_ASSIGN; }
"+="		{ return (int)TokenType::ADD_ASSIGN; }
"-="		{ return (int)TokenType::SUB_ASSIGN; }
"*="		{ return (int)TokenType::MUL_ASSIGN; }
"/="		{ return (int)TokenType::DIV_ASSIGN; }
"%="		{ return (int)TokenType::MOD_ASSIGN; }
"&="		{ return (int)TokenType::AND_ASSIGN; }
"^="		{ return (int)TokenType::XOR_ASSIGN; }
"|="		{ return (int)TokenType::OR_ASSIGN; }
">>"		{ return (int)TokenType::RIGHT_OP; }
"<<"		{ return (int)TokenType::LEFT_OP; }
"++"		{ return (int)TokenType::INC_OP; }
"--"		{ return (int)TokenType::DEC_OP; }
"->"		{ return (int)TokenType::PTR_OP; }
"||"		{ return (int)TokenType::OR_OP; }
"&&"		{ return (int)TokenType::AND_OP; }
"<="		{ return (int)TokenType::LE_OP; }
">="		{ return (int)TokenType::GE_OP; }
"=="		{ return (int)TokenType::EQ_OP; }
"!="		{ return (int)TokenType::NE_OP; }
"{"         { return (int)TokenType::LBRACE; }
";"         { return (int)TokenType::SEMICOLON; }
"}"         { return (int)TokenType::RBRACE; }
","         { return (int)TokenType::COMMA; }
":"         { return (int)TokenType::COLON; }
"="         { return (int)TokenType::EQUAL; }
"("         { return (int)TokenType::LPAREN; }
")"         { return (int)TokenType::RPAREN; }
"["         { return (int)TokenType::LBRACKET; }
"]"         { return (int)TokenType::RBRACKET; }
"."         { return (int)TokenType::DOT; }
"&"         { return (int)TokenType::AMPERSAND; }
"!"         { return (int)TokenType::EXCLAMATION; }
"~"         { return (int)TokenType::TILDE; }
"-"         { return (int)TokenType::MINUS; }
"+"         { return (int)TokenType::PLUS; }
"*"         { return (int)TokenType::ASTERISK; }
"/"         { return (int)TokenType::SLASH; }
"%"         { return (int)TokenType::PERCENT; }
"<"         { return (int)TokenType::LESS_THAN; }
">"         { return (int)TokenType::GREATER_THAN; }
"^"         { return (int)TokenType::CARET; }
"|"         { return (int)TokenType::PIPE; }
"?"         { return (int)TokenType::QUESTION_MARK; }




{string}        { return (int)TokenType::STRING_LITERAL; }

{int_constant}        { return (int)TokenType::INTEGER_CONSTANT; }

{float_constant}      {return (int)TokenType::FLOAT_CONSTANT;}

{identifier}    { return (int)TokenType::IDENTIFIER; }

[ \t\n\r]+      /* Ignore whitespace */

.               {
    throw LexerException("some file", lineno(), "Unexpected token " + std::string(yytext));
    return (int)TokenType::UNKNOWN; }

"//".*          { /* Ignore single-line comments */ }

"/*"            { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n    { }
<COMMENT>.    { }

%%