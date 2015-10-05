%option c++
%option noyywrap

%{
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

FlexLexer* lexer = new yyFlexLexer();

enum yytokentype {
	NUMBER,
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

IDENT   [A-Za-z_][A-Za-z0-9_]*
NUMBER	[0-9]+(\.[0-9]+)?
WHITESPACE  [ \t\f\r]

ADD		"+"
MUL		"*"
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


%%

{ADD}	{
                return ADD;
			}

{MUL} {
				return MUL;
			}

{SUB} {
                return SUB;
			}

{DIV} {
                return DIV;
			}

{NUMBER}		{
                return NUMBER;
			}

{IDENT}     {   
                return ID;
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

{WHITESPACE}     {}
.           { }    

%%