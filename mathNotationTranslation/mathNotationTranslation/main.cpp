#include <stdio.h>
#include "lex.yy.cc"

int main(int argc, char** argv) {
	filebuf fb;
	istream is(fb.open("input.txt", ios::in));
	lexer->yylex(&is, &cout);
	std::cout << "EOF" << std::endl;
}