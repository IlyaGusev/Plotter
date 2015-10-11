#include <iostream>
#include <memory>
#include <utility>
#include <stdexcept>
#include "Parser.h"
using std::cout;
using std::endl;
using std::unique_ptr;
using std::invalid_argument;

int main(int argc, char** argv) {
	try{
		filebuf fb;
		string filename, from, to;
		if (argc == 1){
			filename = "Examples/ml.txt";
			from = "mathml";
			to = "tex";
		}
		else{
			if (argc!=4){
				throw new invalid_argument("Not enough arguments");
			}
			filename = argv[1];
			from = argv[2];
			to = argv[3];
			if (!(from == "mathml" || from == "openmath" || from == "tex") ||
				  !(to == "mathml" || to == "openmath" || to == "tex") ||
					from.compare(to) == 0){
				throw new invalid_argument("Wrong arguments");
			}
		}

		istream is(fb.open(filename, ios::in));
		unique_ptr<Node> tree;
		Parser parser;
		tree.reset();

		while (!parser.processText(is, tree));

		int notation = 0;
		if (to == "mathml")
			notation = 0;
		else if (to == "openmath")
			notation = 1;
		else if (to == "tex")
			notation = 2;
		cout << parser.prev.get()->Translate( notation );
		
		fb.close();
	}
	catch (invalid_argument* e){
		cout << e->what() << endl;
		delete e;
		return -1;
	}
	catch (...){
		cout << "Something went wrong" << endl;
		return -1;
	}
}
