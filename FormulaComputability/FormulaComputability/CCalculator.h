#ifndef CCALCULATOR_H
#define CCALCULATOR_H

#include <cmath>
#include <complex>
#include <vector>

enum STATUS_RESULT {CALCULATED, UNCALCULATED, SUBSPACE};
//UNCALCULATED - метод не сошёлся за 10^7 итераций либо диагональ матрицы содержит нулевой элемент

class CCalculator {

public:

    //ax^2 + bx + c = d;
    static std::pair< std::complex <double>, std::complex <double> > quadraticEquation( double a, double b, double c, double d = 0 );


    // n - размерность матрицы; A[n][n] - матрица коэффициентов, F[n] - столбец свободных членов,
    // X[n] - начальное приближение, ответ записывается также в X[n];
    static STATUS_RESULT Jacobi (int n, const std::vector< std::vector <double> > & A, const std::vector<double>& F, std::vector<double>& X);    

    static int sum( int i_begin, int i_end );

    static int prod( int i_begin, int i_end );


};


class IncorrectArgumentException: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Incorrect arguments";
  }
};

#endif // CCALCULATOR_H
