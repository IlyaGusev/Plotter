%{
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "Node.h"
#include "openmath.tab.h"
using std::cout;
using std::endl;
extern FILE * omin;
extern int NOTATION;
extern std::ofstream FOUT;
int omlex();
void omerror(const char *){};
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
%token <str> ADD MUL SUB DIV EQ SQRT USUB LOBJ ROBJ LAPP RAPP LNUM RNUM LID RID END_OF_FILE

%type <node> stm
%type <composite_node> list
%type <binop_node> binop;

%%
list: list stm { $$ = new CompositeNode($1); }
    | stm { $$ = $1; $$-> add($2); }
;

stm: LNUM stm RNUM { $$ = $2; }
   | LID stm RID { $$ = $2; }
   | LOBJ list ROBJ { $$ = $2; }
   | LAPP list RAPP { $$ = $2; }
   | NUMBER {$$ = new NumNode(omlval.intValue); }
;

binop: ADD stm stm { $$ = new BinOpNode($2, $3, "+"); }
     | SUB stm stm { $$ = new BinOpNode($2, $3, "-"); }
     | MUL stm stm { $$ = new BinOpNode($2, $3, "*"); }
     | DIV stm stm { $$ = new BinOpNode($2, $3, "/"); }
     | EQ stm stm { $$ = new BinOpNode($2, $3, "="); }
;
%%
