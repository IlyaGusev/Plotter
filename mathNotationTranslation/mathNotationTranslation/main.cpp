#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <stdexcept>
#include <fstream>
using namespace std;
extern int NOTATION;
extern ofstream FOUT;
extern int mlparse();
extern FILE *mlin;
extern int texparse();
extern FILE *texin;


int main(int argc, char** argv) {
	try{
		string input, from, to, output;
		if (argc!=5){
			throw new invalid_argument("Not enough arguments");
		}
		input = argv[1];
		output = argv[2];
		from = argv[3];
		to = argv[4];
		if (!(from == "mathml" || from == "openmath" || from == "tex") ||
			  !(to == "mathml" || to == "openmath" || to == "tex") ||
				from.compare(to) == 0){
			throw new invalid_argument("Wrong arguments");
		}

		int to_notation = 0;
		if (to == "mathml")
			to_notation = 0;
		else if (to == "openmath")
			to_notation = 1;
		else if (to == "tex")
			to_notation = 2;
		NOTATION = to_notation;

		if (from == "mathml"){
			mlin = fopen(input.c_str(), "r");
			if (mlin == NULL)
				throw new invalid_argument("File not found");
			FOUT.open(output, std::ofstream::out);
			mlparse();
			fclose(mlin);
		}
		else if (from == "tex"){
			texin = fopen(input.c_str(), "r");
			if (texin == NULL)
				throw new invalid_argument("File not found");
			FOUT.open(output, std::ofstream::out);
			texparse();
			fclose(texin);
		}
		else if (from == "openmath")
			cout << "Not implemented yet :(";
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
