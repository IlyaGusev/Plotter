//#include "FormulaComputability.h"
#include "../pugixml/pugixml.hpp"
#include <iostream>
#include <string>
#include <exception>
#include "FormulaComputability.h"

using namespace std;

class XmlLoadException : public exception 
{
public:
    XmlLoadException( const char* errInfo ) : errorMsg( errInfo ) {};

    const char* what() const noexcept
    {
        return errorMsg.c_str();
    }
private:
    string errorMsg;
};


int main(int argc, char** argv) {
    try {
        char* pathToFile = argv[1];
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(pathToFile);
        if ( !result ) {
            throw XmlLoadException(result.description());
        }
        if ( FormulaComputability::checkComputability(doc) ) {
            cout << "Given formula is computable" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}