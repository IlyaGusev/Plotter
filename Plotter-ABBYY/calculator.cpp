#include "calculator.h"
#include <string>
#include <random>
#include <chrono>
#include <algorithm>

pugi::xml_document MathMlCalculator::doc;
double MathMlCalculator::eps = 0.005;

MathMlCalculator::MathMlCalculator( const wchar_t* formulaPath, bool _is2D ) :
	is2D( _is2D )
{
	pugi::xml_parse_result result = doc.load_file( formulaPath );
	buildFormulas( doc );
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

double MathMlCalculator::GetX( int i, int j ) const
{
	return xPoints[i][j];
}

double MathMlCalculator::GetY( int i, int j ) const
{
	return yPoints[i][j];
}

std::vector<double> MathMlCalculator::GetZ( int i, int j ) const
{
	return zPoints[i][j];
}

bool MathMlCalculator::Is2D()
{
	return is2D;
}

double MathMlCalculator::calculateMax(const std::vector<std::vector<double>>& points) const {
	int n = points.size();
	double max = -INT_MAX;
	for (int i = 0; i < n; ++i) {
		int innerN = points[i].size();
		for (int j = 0; j < innerN; ++j) {
			if (points[i][j] > max) {
				max = points[i][j];
			}
		}
	}
	return max;
}

double MathMlCalculator::calculateZMax( const std::vector<std::vector<std::vector<double>>>& points ) const
{
	int n = points.size();
	double max = -INT_MAX;
	for( int i = 0; i < n; ++i ) {
		int innerN = points[i].size();
		for( int j = 0; j < innerN; ++j ) {
			for( int k = 0; k < points[i][j].size(); ++k ) {
				if( points[i][j][k] > max ) {
					max = points[i][j][k];
				}
			}
		}
	}
	return max;
}

double MathMlCalculator::getXMax() const
{
	double xMax;
	if (!isParametric) {
		xMax = *((--xPoints.end())->begin());
	}
	else {
		xMax = calculateMax(xPoints);
	}
	return xMax;
}

double MathMlCalculator::getYMax() const
{
	double yMax;
	if (!isParametric) {
		yMax = *(--((--yPoints.end())->end()));
	}
	else {
		yMax = calculateMax(yPoints);
	}
	return yMax;
}

double MathMlCalculator::getZMax() const
{
	return calculateZMax( zPoints );
}

void MathMlCalculator::buildFormulas( const pugi::xml_node& formulaRoot )
{
	pugi::xml_node coordDefinitionNode = formulaRoot.first_child();
	if( !coordDefinitionNode.empty() && !buildCoordFormula(coordDefinitionNode) ) {
		// Неявная функция
		coordDefinitionNode = coordDefinitionNode.first_child().next_sibling();
		zFormula = [coordDefinitionNode] () -> double {
			return OperationHandler::getOperation( coordDefinitionNode.name() ).build( coordDefinitionNode ) -
				OperationHandler::getOperation( coordDefinitionNode.next_sibling().name() ).build( coordDefinitionNode.next_sibling() );
		};
		isImplisit = true;
	} else {
		coordDefinitionNode = coordDefinitionNode.next_sibling();
		while( !coordDefinitionNode.empty() && !buildCoordFormula( coordDefinitionNode ) ) {
			// Если встретились еще равенства - значит, формула задана параметрически
			coordDefinitionNode = coordDefinitionNode.next_sibling();
			isParametric = true;
		}
	}
}


bool MathMlCalculator::buildCoordFormula(const pugi::xml_node& coordRoot) {
	pugi::xml_node coordIdentNode = coordRoot.first_child().next_sibling();
	std::string coordName = coordIdentNode.text().as_string();
	if (coordName == "x") {
		xFormula = [coordIdentNode]() -> double {
			return OperationHandler::getOperation(coordIdentNode.next_sibling().name()).build(coordIdentNode.next_sibling());
		};
		return true;
	} else if (coordName == "y") {
		yFormula = [coordIdentNode]() -> double {
			return OperationHandler::getOperation(coordIdentNode.next_sibling().name()).build(coordIdentNode.next_sibling());
		};
		return true;
	} else if (coordName == "z") {
		zFormula = [coordIdentNode]() -> double {
			return OperationHandler::getOperation(coordIdentNode.next_sibling().name()).build(coordIdentNode.next_sibling());
		};
		return true;
	}
	return false;
}

int MathMlCalculator::GetGridSize() const
{
	return gridSize;
}

double MathMlCalculator::getSecondArg( int i, int j )
{
	return (double) (j - gridSize / 2) / 4;
}

bool MathMlCalculator::findRoot( const std::function<double(double)>& func, double& root )
{
	double z_0 = rand() % 100 - 50;
	double z_1 = rand() % 100 - 50;
	while( z_1 == z_0 ) {
		z_1 = rand() % 100 - 50;
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
	if( fabs( z_1 - z_0 ) <= eps && fabs( f_n ) <= 10 * eps ) {
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