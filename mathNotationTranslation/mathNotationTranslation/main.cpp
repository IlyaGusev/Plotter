#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <stdexcept>
using namespace std;

extern int NOTATION;
extern int yyparse();
extern FILE *yyin;

int main(int argc, char** argv) {
	try{
		string filename, from, to;
		if (argc == 1){
			// filename = "C:\\Users\\anata_000\\Source\\Repos\\mathNotationTranslation\\mathNotationTranslation\\mathNotationTranslation\\Examples\\ml.txt";
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

		int notation = 0;
		if (to == "mathml")
			notation = 0;
		else if (to == "openmath")
			notation = 1;
		else if (to == "tex")
			notation = 2;

		NOTATION = notation;

		yyin = fopen(filename.c_str(), "r");
		yyparse();
		cout<<endl;
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
