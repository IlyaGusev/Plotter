#include "CCalculator.h"

//ax^2 + bx + c = d;
std::pair< std::complex <double>, std::complex <double> >  CCalculator::quadraticEquation( double a, double b, double c, double d )
{
    c = c - d;
    if ( b*b - 4*a*c > 0) {
        std::complex<double> x( ( -b + sqrt( b*b - 4*a*c ) ) / ( 2 * a ) );
        std::complex<double> y( ( -b - sqrt( b*b - 4*a*c ) ) / ( 2 * a ) );
        return std::make_pair< std::complex <double>, std::complex <double> >( x, y );
    } else {
        std::complex<double> x( -b / ( 2 * a ), ( sqrt( 4*a*c - b*b ) ) / ( 2 * a ) );
        std::complex<double> y( -b / ( 2 * a ),  ( -sqrt( 4*a*c - b*b ) ) / ( 2 * a ) );
        return std::make_pair< std::complex <double>, std::complex <double> >( x, y );
    }
}
