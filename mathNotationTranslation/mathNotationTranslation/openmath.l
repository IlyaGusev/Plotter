%{
#include <stdio.h>
%}

WHITESPACE  	[ \t\f\r]
LINE_SEPARATOR 	[\n]+
LOBJ			"<OMOBJ>"
ROBJ			"</OMOBJ>"
LAPP			"<OMA>"
RAPP			"</OMA>"
LNUM 			"<OMI>"
RNUM 			"</OMI>"
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
{NUMBER}					{ printf("NUM\n"); }
{LOBJ}						{ printf("LOBJ\n"); }
{ROBJ}						{ printf("ROBJ\n"); }
{LAPP}						{ printf("LAPP\n"); }
{RAPP}						{ printf("RAPP\n"); }
{LNUM}						{ printf("LNUM\n"); }
{RNUM}						{ printf("RNUM\n"); }
{EQUALS}					{ printf("'='\n"); }
{MINUS}						{ printf("'-'\n"); }
{PLUS}						{ printf("'+'\n"); }
{DIVIDE}					{ printf("'/'\n"); }
{TIMES}						{ printf("'*'\n"); }
{ROOT}						{ printf("'sqrt'\n"); }
{UMINUS}					{ printf("unary '-'\n"); }
\<OMV\ name=\"{ID}\"\/>		{ printf("VAR\n"); }

{WHITESPACE}     			{}
{LINE_SEPARATOR} 			{}
.							{}
%%