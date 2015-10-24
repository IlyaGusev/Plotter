%{
#include <stdio.h>
%}

WHITESPACE  	[ \t\f\r]
LINE_SEPARATOR 	[\n]+
LOBJ			"<OMOBJ>"
ROBJ			"</OMOBJ>"
LAPP			"<OMA>"
RAPP			"</OMA>"
LID 			"<OMI>"
RID 			"</OMI>"
ID				[a-z][a-z0-9]*
NUMBER			([0-9]+)(\.[0-9]+)?

%%
{LOBJ}											{ printf("LOBJ\n"); }
{ROBJ}											{ printf("ROBJ\n"); }
{LAPP}											{ printf("LAPP\n"); }
{RAPP}											{ printf("RAPP\n"); }
{LID}											{ printf("LID\n"); }
{RID}											{ printf("RID\n"); }
\<OMV\ name=\"{ID}\"\/>							{ printf("VAR\n"); }
\<OMS\ cd=\"arith1\"\ name=\"minus\"\/>			{ printf("'-'"); }
\<OMS\ cd=\"arith1\"\ name=\"plus\"\/>			{ printf("'+'"); }
\<OMS\ cd=\"arith1\"\ name=\"divide\"\/>		{ printf("'/'"); }
\<OMS\ cd=\"arith1\"\ name=\"times\"\/>			{ printf("'*'"); }
\<OMS\ cd=\"arith1\"\ name=\"root\"\/>			{ printf("'sqrt'"); }
\<OMS\ cd=\"arith1\"\ name=\"unary_minus\"\/>	{ printf("unary '-'"); }


{WHITESPACE}     								{}
{LINE_SEPARATOR} 								{}
%%