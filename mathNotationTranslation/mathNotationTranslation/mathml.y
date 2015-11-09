%{
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "Node.h"
#include "mathml.tab.h"
using std::cout;
using std::endl;
extern FILE * mlin;
extern int NOTATION;
extern std::ofstream FOUT;
int mllex();
void mlerror(const char *){};
%}

%union {
    float value;
    char identName[256];
    char str[256];
    struct Node* node;
    struct CompositeNode* composite_node;
    struct BinOpNode* binop_node;
};

%token <value> NUMBER
%token <identName> ID
%token <str> ADD MUL SUB DIV EQ LOPER ROPER LNUM RNUM LID RID LROW RROW LROOT RROT LFRAC RFRAC LSQRT RSQRT LSUP RSUP LFENCED RFENCED END_OF_FILE
%left LOPER ADD ROPER EQ

%type <node> stm
%type <composite_node> list
%type <binop_node> binop;
%%

list: stm {$$ = new CompositeNode($1);}
 | list stm {$$ = $1; $$->add($2);}
;

stm: LNUM stm RNUM {$$ = $2;}
 | LID stm RID {$$ = $2;}
 | LFRAC stm stm RFRAC {$2->setFence("{", "}"); $3->setFence("{", "}"); $$ = new BinOpNode($2, $3, "frac");}
 | LFENCED list RFENCED {$$ = $2; $$->setFence("(", ")");}
 | LROW list RROW {$$ = $2;}
 | LROW list RROW END_OF_FILE {$$ = $2; FOUT<<$$->translate(NOTATION); }
 | binop {$$ = $1;}
 | NUMBER {$$ = new NumNode(mllval.value);}
 | ID {$$ = new IdNode(mllval.identName);}
 | LSUP stm stm RSUP {$$ = new BinOpNode($2, $3, "sup");}
;

binop: stm LOPER ADD ROPER stm { $$ = new BinOpNode($1, $5, "+");}
 | stm LOPER EQ ROPER stm { $$ = new BinOpNode($1, $5, "=");}
 | stm LOPER SUB ROPER stm { $$ = new BinOpNode($1, $5, "-");}
 | stm LOPER MUL ROPER stm { $$ = new BinOpNode($1, $5, "*");}
 | stm LOPER DIV ROPER stm { $$ = new BinOpNode($1, $5, "/");}
;
%%
