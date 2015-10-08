%option c++
%option noyywrap

%{
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

FlexLexer* lexer = new yyFlexLexer();

string value;

enum yytokentype {
	NUMBER = 258,
	ID,
	ADD,
	MUL,
	SUB,
	DIV,
	LOPER,
	ROPER,
	LNUM,
	RNUM,
	LID,
	RID,
	LROW,
	RROW,
	LROOT,
	RROOT,
	LFRAC,
	RFRAC,
	LSQRT,
	RSQRT,
	LSUP,
	RSUP
   };

%}

WHITESPACE  [ \t\f\r]

SUB		"-"
DIV		"/"
LOPER	"<mo>"
ROPER	"</mo>"
LNUM	"<mn>"
RNUM	"</mn>"
LID		"<mi>"
RID		"</mi>"
LROW	"<mrow>"
RROW	"</mrow>"
LROOT	"<mroot>"
RROOT	"</mroot>"
LFRAC	"<mfrac>"
RFRAC	"</mfrac>"
LSQRT	"<msqrt>"
RSQRT	"</msqrt>"
LSUP	"<msup>"
RSUP	"</msup>"
ID	[a-zA-Z][a-zA-Z0-9]*
NUMBER	([0-9]+)(\.[0-9]+)?


%%
{ID}	{
			value = yytext;
			return ID;
		}

"+"		{
			return ADD;
		}

"*"		{
				return MUL;
		}

{SUB}	{
                return SUB;
			}

{DIV}	{
                return DIV;
			}

{NUMBER}	{
                value = yytext;
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