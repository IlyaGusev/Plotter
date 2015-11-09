%option noyywrap
%option prefix="ml"
%{
#include <fstream>
#include "mathml.tab.h"
int NOTATION = 0;
std::ofstream FOUT;

//FlexLexer* lexer = new mlFlexLexer();
void mlerror(const char *);
%}

WHITESPACE  	[ \t\f\r]
LINE_SEPARATOR 	[\n]+
LOPER			"<mo>"
ROPER			"</mo>"
LNUM			"<mn>"
RNUM			"</mn>"
LID				"<mi>"
RID				"</mi>"
LROW			"<mrow>"
RROW			"</mrow>"
LROOT			"<mroot>"
RROOT			"</mroot>"
LFRAC			"<mfrac>"
RFRAC			"</mfrac>"
LSQRT			"<msqrt>"
RSQRT			"</msqrt>"
LSUP			"<msup>"
RSUP			"</msup>"
LFENCED 		"<mfenced>"
RFENCED 		"</mfenced>"
ID				[a-zA-Z][a-zA-Z0-9]*
NUMBER			([0-9]+)(\.[0-9]+)?
END_OF_FILE 	<<EOF>>


%%
{ID}	{strcpy(mllval.identName, mltext);return ID;}
"+"		{return ADD;}
"*"		{return MUL;}
"-"		{return SUB;}
"/"		{return DIV;}
"="		{return EQ;}

{NUMBER}	{    mllval.value = atof(mltext);	return NUMBER;}
{LOPER}     {return LOPER;}
{ROPER}     {return ROPER;}
{LID}     {return LID;}
{RID}     {return RID;}
{LROW}     {return LROW;}
{RROW}     {return RROW;}
{LFRAC}     {return LFRAC;}
{RFRAC}     {return RFRAC;}
{LSQRT}     {return LSQRT;}
{RSQRT}     {return RSQRT;}
{LSUP}     {return LSUP;}
{RSUP}     {return RSUP;}
{LNUM}     {return LNUM;}
{RNUM}     {return RNUM;}
{LFENCED}     {return LFENCED;}
{RFENCED}     {return RFENCED;}
{WHITESPACE}     {}
.           { }

{LINE_SEPARATOR} {}

<<EOF>>     { return END_OF_FILE;}

%%
