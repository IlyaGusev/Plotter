%option noyywrap
%option prefix="om"
%{
#include <stdio.h>
#include "openmath.tab.h"
//FlexLexer* lexer = new omFlexLexer();
void omerror(const char *);
%}

WHITESPACE  	[ \t\f\r]
LINE_SEPARATOR 	[\n]+
LOBJ			"<OMOBJ>"
ROBJ			"</OMOBJ>"
LAPP			"<OMA>"
RAPP			"</OMA>"
LNUM 			"<OMI>"
RNUM 			"</OMI>"
LID				\<OMV\ name=\"
RID				\"\/>
EQUALS			\<OMS\ cd=\"relation1\"\ name=\"eq\"\/>
MINUS 			\<OMS\ cd=\"arith1\"\ name=\"minus\"\/>
PLUS 			\<OMS\ cd=\"arith1\"\ name=\"plus\"\/>
DIVIDE			\<OMS\ cd=\"arith1\"\ name=\"divide\"\/>
TIMES 			\<OMS\ cd=\"arith1\"\ name=\"times\"\/>
ROOT			\<OMS\ cd=\"arith1\"\ name=\"root\"\/>
UMINUS 			\<OMS\ cd=\"arith1\"\ name=\"unary_minus\"\/>
ID				[a-z][a-z0-9]*
NUMBER			([0-9]+)(\.[0-9]+)?
END_OF_FILE		<<EOF>>

%%
{ID}			{ printf("ID\n"); strcpy(omlval.identName, omtext); return ID; }
{NUMBER}		{ printf("NUM\n"); omlval.intValue = atoi(mltext); return NUMBER; }
{LOBJ}			{ printf("LOBJ\n"); return LOBJ; }
{ROBJ}			{ printf("ROBJ\n"); return ROBJ; }
{LAPP}			{ printf("LAPP\n"); return LAPP; }
{RAPP}			{ printf("RAPP\n"); return RAPP; }
{LNUM}			{ printf("LNUM\n"); return LNUM; }
{RNUM}			{ printf("RNUM\n"); return RNUM; }
{LID}			{ printf("LID\n"); return LID; }
{RID}			{ printf("RID\n"); return RID; }

{EQUALS}		{ printf("'='\n"); return EQ; }
{MINUS}			{ printf("'-'\n"); return SUB; }
{PLUS}			{ printf("'+'\n"); return ADD; }
{DIVIDE}		{ printf("'/'\n"); return DIV; }
{TIMES}			{ printf("'*'\n"); return MUL; }
{ROOT}			{ printf("'sqrt'\n"); return SQRT; }
{UMINUS}		{ printf("unary '-'\n"); return USUB; }


{WHITESPACE}     			{}
{LINE_SEPARATOR} 			{}
.							{}
<<EOF>>						{ return END_OF_FILE; }
%%