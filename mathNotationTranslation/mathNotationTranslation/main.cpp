#include "Parser.h"
#include "arithmeticNode.h"
#include "variableNode.h"
int main(int argc, char** argv) {
	filebuf fb;
	argv[1] = "input.txt";
	argv[2] = "MathML";
	argv[3] = "MathML";
	istream is(fb.open(argv[1], ios::in));
	Parser parser(argv[2], argv[3]);
	parser.processText(is, 0);
}