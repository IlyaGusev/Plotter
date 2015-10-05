%option c++
%option noyywrap

%{
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

FlexLexer* lexer = new yyFlexLexer();
int curLineNum = 0;
int curPosInLine = 0;

void incrCurPos(int symNumber) {
    curPosInLine += symNumber;
}

%}

IDENT   [A-Za-z_][A-Za-z0-9_]*
INT     [0-9]+
FLOAT	[0-9]+(\.[0-9]+)?
SPACE   [ \t]+
LINE_SEPARATOR [\n]+


%%

{INT}       {
                std::cout << "INT(" << atoi(yytext) << ") ";
            }


{FLOAT}		{
                std::cout << "FLOAT(" << atof(yytext) << ") ";
			}

{IDENT}     {   
                curPosInLine += yyleng;
                std::cout << "IDENT(" << yytext << ") ";
            }

{SPACE}     {   ++curPosInLine;    }

{LINE_SEPARATOR}    {}
.           { }       

%%