%option c++
%option noyywrap

%{
FlexLexer* lexer = new yyFlexLexer();

string value;

enum yytokentype {
	NUMBER = 258,
	ID,
	ADD,
	MUL,
	SUB,
	DIV,
	EQ,
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
			value = yytext;
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
