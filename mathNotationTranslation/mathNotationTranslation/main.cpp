#include <iostream>
#include <memory>
#include <utility>
#include "Parser.h"
using std::cout;
using std::endl;
using std::unique_ptr;

int main(int argc, char** argv) {
	filebuf fb;
	argv[1] = "input.txt";
	argv[2] = "mathml";
	argv[3] = "tex";
	istream is(fb.open(argv[1], ios::in));
	unique_ptr<Node> tree;
	Parser parser;
	tree.reset();

	while (!parser.processText(is, tree));
	cout << parser.prev.get()->Translate( 2 );
}
