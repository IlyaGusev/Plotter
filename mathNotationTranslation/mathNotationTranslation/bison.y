%{
#include <string>
#include <stdio.h>
#include <iostream>
#include "bison.tab.h"
extern FILE * yyin;
int yylex();
void yyerror(const char *){};
%}

%union {
    int intValue;
    char identName[256];
    char str[256];
};

%token <intValue> NUMBER
%token <identName> ID
%token <str> ADD MUL SUB DIV EQ LOPER ROPER LNUM RNUM LID RID LROW RROW LROOT RROT LFRAC RFRAC LSQRT RSQRT LSUP RSUP
%left LOPER
%%

list: stm
 | list stm
;

stm: LNUM stm RNUM
 | LID stm RID
 | LFRAC LROW list RROW LROW list RROW RFRAC {std::cout<<"!frac rule end!";}
 | LROW list RROW {std::cout<<"!row rule end!";}
 | binop {std::cout<<"!binop rule end!";}
 | NUMBER {std::cout << yylval.intValue << " ";}
 | ID { std::cout << yylval.identName << " ";}
;

binop: stm LOPER ADD ROPER stm {std::cout << "+" << " ";}
 | stm LOPER EQ ROPER stm { std::cout << "=" << " ";}
;
%%
