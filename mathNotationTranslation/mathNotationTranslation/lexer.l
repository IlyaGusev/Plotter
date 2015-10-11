%option noyywrap

%{
#include "bison.tab.h"
//FlexLexer* lexer = new yyFlexLexer();
void yyerror(const char *);
%}

WHITESPACE  [ \t\f\r]
LINE_SEPARATOR [\n]+

LOPER	"<mo>"
ROPER	"</mo>"
LNUM	"<mn>"
RNUM	"</mn>"
LID		"<mi>"
RID		"</mi>"
LROW	("<mrow>"|"{")
RROW	("</mrow>"|"}")
LROOT	"<mroot>"
RROOT	"</mroot>"
LFRAC	("<mfrac>"|"\\frac")
RFRAC	"</mfrac>"
LSQRT	"<msqrt>"
RSQRT	"</msqrt>"
LSUP	"<msup>"
RSUP	"</msup>"
ID	[a-zA-Z][a-zA-Z0-9]*
NUMBER	([0-9]+)(\.[0-9]+)?


%%
{ID}	{
			strcpy(yylval.identName, yytext);
			return ID;
		}

"+"		{
			return ADD;
		}

"*"		{
				return MUL;
		}

"-"		{
                return SUB;
		}

"/"		{
                return DIV;
		}

"="		{
				return EQ;
		}

{NUMBER}	{
        yylval.intValue = atoi(yytext);
				return NUMBER;
			}

{LOPER}     {
                return LOPER;
            }
{ROPER}     {
                return ROPER;
            }
{LID}     {
                return LID;
            }
{RID}     {
                return RID;
            }
{LROW}     {
                return LROW;
            }
{RROW}     {
                return RROW;
            }
{LFRAC}     {
                return LFRAC;
            }
{RFRAC}     {
                return RFRAC;
            }
{LSQRT}     {
                return LSQRT;
            }
{RSQRT}     {
                return RSQRT;
            }
{LSUP}     {
                return LSUP;
            }
{RSUP}     {
                return RSUP;
            }
{LNUM}     {
                return LNUM;
            }
{RNUM}     {
                return RNUM;
            }

{WHITESPACE}     {}
.           { }

%%
