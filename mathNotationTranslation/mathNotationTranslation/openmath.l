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
LID				"<OMV name=\""
RID				"\"/>"
EQUALS			"<OMS\ cd=\"relation1\"\ name=\"eq\"\/>"
MINUS 			"<OMS\ cd=\"arith1\"\ name=\"minus\"\/>"
PLUS 			"<OMS\ cd=\"arith1\"\ name=\"plus\"\/>"
DIVIDE			"<OMS\ cd=\"arith1\"\ name=\"divide\"\/>"
TIMES 			"<OMS\ cd=\"arith1\"\ name=\"times\"\/>"
ROOT			"<OMS\ cd=\"arith1\"\ name=\"root\"\/>"
UMINUS 			"<OMS\ cd=\"arith1\"\ name=\"unary_minus\"\/>"
ID				[a-z][a-z0-9]*
NUMBER			([0-9]+)(\.[0-9]+)?
END_OF_FILE		<<EOF>>

%%
{ID}			{ strcpy(omlval.identName, omtext); return ID; }
{NUMBER}		{ omlval.value = atof(omtext); return NUMBER; }
{LOBJ}			{ return LOBJ; }
{ROBJ}			{ return ROBJ; }
{LAPP}			{ return LAPP; }
{RAPP}			{ return RAPP; }
{LNUM}			{ return LNUM; }
{RNUM}			{ return RNUM; }
{LID}			{ return LID; }
{RID}			{ return RID; }

{EQUALS}		{ return EQ; }
{MINUS}			{ return SUB; }
{PLUS}			{ return ADD; }
{DIVIDE}		{ return DIV; }
{TIMES}			{ return MUL; }
{ROOT}			{ return SQRT; }
{UMINUS}		{ return USUB; }


{WHITESPACE}     			{}
{LINE_SEPARATOR} 			{}
.							{}
<<EOF>>						{ return END_OF_FILE; }
%%