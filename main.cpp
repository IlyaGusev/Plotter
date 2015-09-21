//
//  main.cpp
//  FormulaComputability
//
//  Created by Даниил Назаров on 20.09.15.
//  Copyright © 2015 Даниил Назаров. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sbml/SBMLTypes.h>
using namespace std;


int main() {

    const char* xmlFile = "<?xml version='1.0' encoding='UTF-8'?>"
    "<math xmlns='http://www.w3.org/1998/Math/MathML'>"
    "  <apply> <plus/> <cn> 1 </cn>"
    "                  <apply> <ci> f </ci> <ci> x </ci> </apply>"
    "  </apply>"
    "</math>";

    ASTNode* ast = readMathMLFromString(xmlFile);
    char* result = SBML_formulaToString(ast);
    cout << "Example of parsing MathML xml file:" << endl << result << endl;

    return 0;
}
