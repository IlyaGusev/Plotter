%option noyywrap
%option prefix="tex"
%{
#include "tex.tab.h"
//FlexLexer* lexer = new texFlexLexer();
void texerror(const char *);
%}

WHITESPACE  [ \t\f\r]
LINE_SEPARATOR [\n]+

LBRACE	"{"
RBRACE  "}"
FRAC	"\\frac"
SQRT	"\\sqrt"
LPAREN "("
RPAREN ")"
ID	[a-zA-Z][a-zA-Z0-9]*
NUMBER	([0-9]+)(\.[0-9]+)?
END_OF_FILE <<EOF>>


%%
"+"		{ return ADD; }
"*"		{ return MUL; }
"-"		{ return SUB; }
"/"		{ return DIV; }
"="		{ return EQ; }
"^"		{ return SUP; }

{ID}		 { strcpy(texlval.identName, textext); return ID; }
{NUMBER} { texlval.value = atof(textext); return NUMBER; }
{LBRACE} { return LBRACE; }
{RBRACE} { return RBRACE; }
{FRAC}   { return FRAC; }
{SQRT}   { return SQRT; }
{LPAREN} { return LPAREN; }
{RPAREN} { return RPAREN; }
{WHITESPACE} {}
.         {}
{LINE_SEPARATOR} {}
<<EOF>>   { return END_OF_FILE; }
%%
