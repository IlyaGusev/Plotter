#include "calculator.h"
#include <string>
#include <random>
#include <chrono>
#include <algorithm>

pugi::xml_document MathMlCalculator::doc;
double MathMlCalculator::eps = 0.005;

MathMlCalculator::MathMlCalculator( const wchar_t* formulaPath, bool _is2D, bool _isImplisit ) :
	is2D( _is2D ),
	isImplisit( _isImplisit )
{
	pugi::xml_parse_result result = doc.load_file( formulaPath );
	buildFormula( doc );
	srand( time( NULL ) );
}

void MathMlCalculator::RecalculatePoints()
{
	RecalculatePoints( gridSize );
}

void MathMlCalculator::RecalculatePoints( int _gridSize )
{
	gridSize = _gridSize;
	xPoints.resize( gridSize );
	yPoints.resize( gridSize );
	zPoints.resize( gridSize );
	for( int i = 0; i < gridSize; i++ ) {
		if( !is2D ) {
			xPoints[i].resize( gridSize );
			yPoints[i].resize( gridSize );
			zPoints[i].resize( gridSize );
			for( int j = 0; j < gridSize; j++ ) {
				if( !isParametric ) {
					OperationHandler::setVar( "x", getFirstArg( i, j ) );
					xPoints[i][j] = getFirstArg( i, j );
					OperationHandler::setVar( "y", getSecondArg( i, j ) );
					yPoints[i][j] = getSecondArg( i, j );

					if( isImplisit ) {
						zPoints[i][j] = findAllZRoots();
					} else {
						zPoints[i][j].push_back( zFormula() );
					}
				} else {
					OperationHandler::setVar( "t", getFirstArg( i, j ) );
					OperationHandler::setVar( "u", getSecondArg( i, j ) );
					xPoints[i][j] = xFormula();
					yPoints[i][j] = yFormula();
					if( isImplisit ) {
						zPoints[i][j] = findAllZRoots();
					} else {
						zPoints[i][j].push_back( zFormula() );
					}
				}
			}
		} else {
			xPoints[i].resize( 1 );
			zPoints[i].resize( 1 );
			if( !isParametric ) {
				OperationHandler::setVar( "x", getFirstArg( i, 0 ) );
				xPoints[i][0] = getFirstArg( i, 0 );
				if( isImplisit ) {
					zPoints[i][0] = findAllZRoots();
				} else {
					zPoints[i][0].push_back( zFormula() );
				}
			} else {
				OperationHandler::setVar( "t", getFirstArg( i, 0 ) );
				xPoints[i][0] = xFormula();
				if( isImplisit ) {
					zPoints[i][0] = findAllZRoots();
				} else {
					zPoints[i][0].push_back( zFormula() );
				}
			}
		}
	}
}

double MathMlCalculator::GetX( int i, int j )
{
	return xPoints[i][j];
}

double MathMlCalculator::GetY( int i, int j )
{
	return yPoints[i][j];
}

std::vector<double> MathMlCalculator::GetZ( int i, int j )
{
	return zPoints[i][j];
}

bool MathMlCalculator::Is2D()
{
	return is2D;
}

void MathMlCalculator::buildFormula( const pugi::xml_node& formulaRoot )
{
	pugi::xml_node curNode = formulaRoot.first_child();
	if( !curNode.next_sibling().empty() ) {
		xFormula = [curNode] () -> double {
			return OperationHandler::getOperation( curNode.first_child().name() ).build( curNode.first_child() );
		};
		curNode = curNode.next_sibling();
		zFormula = [curNode] () -> double {
			return OperationHandler::getOperation( curNode.first_child().name() ).build( curNode.first_child() );
		};
		isParametric = true;
	} else {
		zFormula = [curNode] () -> double {
			return OperationHandler::getOperation( curNode.name() ).build( curNode );
		};
	}
}

int MathMlCalculator::GetGridSize()
{
	return gridSize;
}

double MathMlCalculator::getSecondArg( int i, int j )
{
	return (double) (j - gridSize / 2) / 4;
}

bool MathMlCalculator::findRoot( const std::function<double(double)>& func, double& root )
{
	double z_0 = rand() % 200 - 100;
	double z_1 = rand() % 200 - 100;
	while( z_1 == z_0 ) {
		z_1 = rand( ) % 200 - 100;
	}
	double z_n = 0;
	for( int i = 0; i < 100 && fabs( z_1 - z_0 ) > eps; ++i ) {
		OperationHandler::setVar( "z", z_0 );
		double f_0 = zFormula() / func( z_0 );
		OperationHandler::setVar( "z", z_1 );
		double f_1 = zFormula() / func( z_1 );

		z_n = z_1 - (z_1 - z_0) / (f_1 - f_0) * f_1;
		z_0 = z_1;
		z_1 = z_n;
	}
	OperationHandler::setVar( "z", z_n );
	double f_n = zFormula() / func( z_n );
	if( fabs( z_1 - z_0 ) <= eps && fabs( f_n ) <= eps ) {
		root = z_n;
		return true;
	}
	return false;
}

std::vector<double> MathMlCalculator::findAllZRoots()
{
	std::vector<double> roots;
	std::function<double( double )> func = [] ( double z ) { return 1; };

	double root;
	while( findRoot( func, root ) ) {
		roots.push_back( root );
		func = [root, func] ( double z ) -> double {
			return func( z ) * (z - root);
		};
	}
	std::sort( roots.begin(), roots.end() );
	return roots;
}

double MathMlCalculator::getFirstArg( int i, int j )
{
	return (double) (i - gridSize / 2) / 4;
}