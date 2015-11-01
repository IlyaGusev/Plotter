#ifndef CCALCULATOR_H
#define CCALCULATOR_H

#include <cmath>
#include <complex>
#include <vector>

enum STATUS_RESULT {INF, CALCULATED, UNCALCULATED, NO_RESULT};

class CCalculator {

public:

    //ax^2 + bx + c = d;
    static std::pair< std::complex <double>, std::complex <double> > quadraticEquation( double a, double b, double c, double d = 0 );


    // n - размерность матрицы; A[n][n] - матрица коэффициентов, F[n] - столбец свободных членов,
    // X[n] - начальное приближение, ответ записывается также в X[n];
    static STATUS_RESULT Jacobi (int n, const std::vector< std::vector <double> > & A, const std::vector<double>& F, std::vector<double>& X);

};

#endif // CCALCULATOR_H
