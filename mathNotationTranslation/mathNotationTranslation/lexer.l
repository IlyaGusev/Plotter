%{
 
/*
 * Lexer.l file
 * To generate the lexical analyzer run: "flex Lexer.l"
 */
 
#include "Test.h"
#include "parser.h"

#include <stdio.h>
 
%}

%option outfile="Lexer.c" header-file="Lexer.h"
%option warn nodefault
 
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge
 
LPAREN      "("
RPAREN      ")"
PLUS        "+"
MULTIPLY    "*"
DIVIDE		"/"
MINUS		"-"
 
NUMBER      [-+]?[0-9]*\.?[0-9]+
WS          [ \r\n\t]*

LNUMBER		"<mn>"
RNUMBER		"</mn>"

LOPER		"<mo>"
ROPER		"</mo>"
 
%%
 
{WS}            { /* Skip blanks. */ }
{NUMBER}/{RNUMBER}        { sscanf_s(yytext, "%d", &yylval->value); return TOKEN_NUMBER; }
 
{MULTIPLY}/{ROPER}     { return TOKEN_MULTIPLY; }
{PLUS}/{ROPER}          { return TOKEN_PLUS; }
{MINUS}/{ROPER}	{ return TOKEN_MINUS; }
{DIVIDE}/{ROPER}	{ return TOKEN_DIVIDE; }
{LPAREN}        { return TOKEN_LPAREN; }
{RPAREN}        { return TOKEN_RPAREN; }
.               {  }
 
%%
