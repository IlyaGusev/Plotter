#include "CCalculator.h"

//ax^2 + bx + c = d;
std::pair< std::complex <double>, std::complex <double> >  CCalculator::quadraticEquation( double a, double b, double c, double d )
{
    c = c - d;
    if ( b*b - 4*a*c > 0) {
        std::complex<double> x( ( -b + sqrt( b*b - 4*a*c ) ) / ( 2 * a ) );
        std::complex<double> y( ( -b - sqrt( b*b - 4*a*c ) ) / ( 2 * a ) );
        return std::pair< std::complex <double>, std::complex <double> >( x, y );
    } else {
        std::complex<double> x( -b / ( 2 * a ), ( sqrt( 4*a*c - b*b ) ) / ( 2 * a ) );
        std::complex<double> y( -b / ( 2 * a ),  ( -sqrt( 4*a*c - b*b ) ) / ( 2 * a ) );
        return std::pair< std::complex <double>, std::complex <double> >( x, y );
    }
}




// n - размерность матрицы; A[n][n] - матрица коэффициентов, F[n] - столбец свободных членов,
// X[n] - начальное приближение, ответ записывается также в X[n];
STATUS_RESULT CCalculator::Jacobi (int n, const std::vector< std::vector <double> >& A, const std::vector<double>& F, std::vector<double>& X)
{
    //проверка данных на валидность
    for ( int i = 0; i < A.size(); ++i ) {
        if ( A[i].size() != n ) {
            throw IncorrectArgumentException();
        }
    }

    double eps = 0.001;
    std::vector< double > tempX(n);
    double norm; // норма, определяемая как наибольшая разность компонент столбца иксов соседних итераций.

    for (int i = 0; i < n; ++i) {
        if (A[i][i] == 0) {
            return UNCALCULATED;
        }
    }

    int counter = 0;
    do {
        ++counter;
        if ( counter > 10000000 ) {
            return UNCALCULATED;
        }
        for (int i = 0; i < n; i++) {
            tempX[i] = F[i];
            for (int g = 0; g < n; g++) {
                if (i != g)
                    tempX[i] -= A[i][g] * X[g];
            }
            tempX[i] /= A[i][i];
        }

        norm = fabs(X[0] - tempX[0]);
        for (int h = 0; h < n; h++) {
            if (fabs(X[h] - tempX[h]) > norm)
                norm = fabs(X[h] - tempX[h]);
            X[h] = tempX[h];
        }
    } while ( norm > eps );
    return CALCULATED;
}



int CCalculator::sum( int i_begin, int i_end ) {
    int result = 0;
    for (int i = i_begin; i <= i_end; ++i) {
        result += i;
    }
    return result;
}

int CCalculator::prod( int i_begin, int i_end ) {
    int result = 1;
    for (int i = i_begin; i <= i_end; ++i) {
        result *= i;
    }
    return result;
}


