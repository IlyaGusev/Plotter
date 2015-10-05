#include <iostream>
#include <fstream>  
#include "lex.yy.cc"


int main(int argc, char** argv) {
	filebuf fb;
	argv[1] = "input.txt";
	istream is(fb.open(argv[1], ios::in));
	int tok;
	while (tok = lexer->yylex(&is, &cout)) {
	}
}