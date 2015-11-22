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
%token <str> ADD MUL SUB DIV EQ SQRT LMATH RMATH LAPP RAPP LNUM RNUM LID RID END_OF_FILE
%left ADD EQ

%type <node> stm
%type <composite_node> list
%type <binop_node> binop;
%%

list: stm {$$ = new CompositeNode($1);}
    | list stm {$$ = $1; $$->add($2);}
;

stm: LNUM stm RNUM {$$ = $2;}
   | LID stm RID {$$ = $2;}
   | LMATH list RMATH { $$ = $2; FOUT<<$$->translate(NOTATION); }
   | LAPP list RAPP { $$ = $2; }
   | binop {$$ = $1;}
   | NUMBER {$$ = new NumNode(mllval.value);}
   | ID {$$ = new IdNode(mllval.identName);}
;

binop: ADD stm stm { $$ = new BinOpNode($2, $3, "+"); }
     | SUB stm stm { $$ = new BinOpNode($2, $3, "-"); }
     | MUL stm stm { $$ = new BinOpNode($2, $3, "*"); }
     | DIV stm stm { $$ = new BinOpNode($2, $3, "/"); }
     | EQ stm stm { $$ = new BinOpNode($2, $3, "="); }
;
%%
