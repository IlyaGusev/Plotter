%{
#include <string>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <utility>
#include <stdexcept>
#include "Node.h"
#include "bison.tab.h"
using std::cout;
using std::endl;
using std::unique_ptr;
extern FILE * yyin;
int yylex();
void yyerror(const char *){};
%}

%union {
    int intValue;
    char identName[256];
    char str[256];
    struct Node* node;
    struct CompositeNode* composite_node;
    struct BinOpNode* binop_node;
};

%token <intValue> NUMBER
%token <identName> ID
%token <str> ADD MUL SUB DIV EQ LOPER ROPER LNUM RNUM LID RID LROW RROW LROOT RROT LFRAC RFRAC LSQRT RSQRT LSUP RSUP
%left LOPER ADD ROPER EQ

%type <node> stm
%type <composite_node> list
%type <binop_node> binop;
%%

list: stm {$$ = new CompositeNode(); $$->add($1);}
 | list stm {$$ = $1; $$->add($2);}
;

stm: LNUM stm RNUM {$$ = $2;}
 | LID stm RID {$$ = $2;}
 | LFRAC LROW list RROW LROW list RROW RFRAC {$$ = new BinOpNode($3, $6, "frac");}
 | LROW list RROW {$$ = $2;}
 | binop {$$ = $1;}
 | NUMBER {$$ = new NumNode(yylval.intValue);}
 | ID {$$ = new IdNode(yylval.identName);}
 | LSUP list RSUP { $$ = $2; cout<<$$->translate(NOTATION);}
;

binop: stm LOPER ADD ROPER stm { $$ = new BinOpNode($1, $5, "+");}
 | stm LOPER EQ ROPER stm { $$ = new BinOpNode($1, $5, "=");}
;
%%
