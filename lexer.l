%{
#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include "parser.tab.h"
#include "ast.h"

extern YYSTYPE yylval;



// Convert char* to std::string* for string tokens
std::string* make_string(const char* text) {
    return new std::string(text);
}

// String literal handling helper
std::string* process_string_literal(const char* text) {
    std::string s(text + 1, strlen(text) - 2); // Remove quotes
    return new std::string(s);
}
%}

%option noyywrap yylineno

%%

"if"      { return IF; }
"else"    { return ELSE; }
"for"     { return FOR; }
"while"   { return WHILE; }
"display" { return DISPLAY; }
"read"    { return READ; }
"true"    { yylval.bval = true; return TRUE; }
"false"   { yylval.bval = false; return FALSE; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); return ID; }
[+-]?[0-9]+"."[0-9]+    { yylval.fval = std::stof(yytext); return FLOAT_LITERAL; }
[+-]?[0-9]+             { yylval.ival = std::stoi(yytext); return INT_LITERAL; }
\"([^\"\\]|\\.)*\"      { yylval.sval = strdup(yytext); return STRING_LITERAL; }

"==" { return EQ; }
"!=" { return NEQ; }
"<=" { return LEQ; }
">=" { return GEQ; }
"<"  { return LT; }
">"  { return GT; }
"&&" { return AND; }
"||" { return OR; }
"!"  { return NOT; }

"+"  { return PLUS; }
"-"  { return MINUS; }
"*"  { return MULT; }
"/"  { return DIV; }
"="  { return ASSIGN; }
"++" { return INCREMENT; }
"--" { return DECREMENT; }

"{" { return LBRACE; }
"}" { return RBRACE; }
"(" { return LPAREN; }
")" { return RPAREN; }
";" { return SEMICOLON; }

[ \t\r\n]+ ; /* Skip whitespace */
"//".*     ; /* Skip single-line comments */
"/*"([^*]|"*"[^/])*"*/"  ; /* Skip multi-line comments */

. {
    std::cerr << "Lexical error at line " << yylineno << ": Unexpected character '" << yytext << "'" << std::endl;
    exit(1);
}

%%
