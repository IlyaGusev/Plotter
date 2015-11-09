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
LMATH			"<math>"
RMATH			"</math>"
LAPP			"<apply>"
RAPP			"</apply>"
LNUM			"<cn>"
RNUM			"</cn>"
LID				"<ci>"
RID				"</ci>"
EQUALS			"<eq/>"
PLUS			"<plus/>"
MINUS			"<minus/>"
TIMES			"<times/>"
DIVIDE			"<divide/>"
ROOT			"<root/>"
ID				[a-zA-Z][a-zA-Z0-9]*
NUMBER			([0-9]+)(\.[0-9]+)?
END_OF_FILE 	<<EOF>>


%%
{ID}			{ strcpy(mllval.identName, mltext); return ID; }
{NUMBER}		{ mllval.value = atof(mltext);	return NUMBER; }
{LMATH}			{ return LMATH; }
{RMATH}			{ return RMATH; }
{LAPP}			{ return LAPP; }
{RAPP}			{ return RAPP; }
{LNUM}     		{ return LNUM;}
{RNUM}     		{ return RNUM;}
{LID}			{ return LID; }
{RID}			{ return RID; }

{EQUALS}		{ return EQ; }
{MINUS}			{ return SUB; }
{PLUS}			{ return ADD; }
{DIVIDE}		{ return DIV; }
{TIMES}			{ return MUL; }
{ROOT}			{ return SQRT; }

{WHITESPACE}     {}
{LINE_SEPARATOR} {}
.                {}
<<EOF>>          { return END_OF_FILE;}

%%
