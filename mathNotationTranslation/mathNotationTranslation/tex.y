%{
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "Node.h"
#include "tex.tab.h"
using std::cout;
using std::endl;
extern FILE * texin;
extern int NOTATION;
extern std::ofstream FOUT;
int texlex();
void texerror(const char *){};
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
%token <str> ADD MUL SUB DIV EQ SUP LBRACE RBRACE FRAC SQRT LPAREN RPAREN END_OF_FILE
%left ADD SUB EQ
%left MUL DIV

%type <node> stm
%type <composite_node> list
%type <binop_node> binop;
%%

list: stm {$$ = new CompositeNode($1);}
 | list stm {$$ = $1; $$->add($2);}
;

stm: FRAC LBRACE list RBRACE LBRACE list RBRACE {$3->setFence("{", "}"); $6->setFence("{", "}"); $$ = new BinOpNode($3, $6, "frac");}
 | LPAREN list RPAREN {$$ = $2; $$->setFence("(", ")");}
 | LBRACE list RBRACE {$$ = $2; FOUT<<$$->translate(NOTATION);}
 | binop {$$ = $1;}
 | NUMBER {$$ = new NumNode(texlval.intValue);}
 | ID {$$ = new IdNode(texlval.identName);}
;

binop: stm ADD stm { $$ = new BinOpNode($1, $3, "+");}
 | stm EQ stm { $$ = new BinOpNode($1, $3, "=");}
 | stm SUB stm { $$ = new BinOpNode($1, $3, "-");}
 | stm MUL stm { $$ = new BinOpNode($1, $3, "*");}
 | stm DIV stm { $$ = new BinOpNode($1, $3, "/");}
;
%%
