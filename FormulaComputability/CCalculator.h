#ifndef CCALCULATOR_H
#define CCALCULATOR_H

#include <cmath>
#include <complex>
#include <vector>

class CCalculator {
public:

    //ax^2 + bx + c = d;
    static std::pair< std::complex <double>, std::complex <double> > quadraticEquation( double a, double b, double c, double d = 0 );
};

#endif // CCALCULATOR_H
