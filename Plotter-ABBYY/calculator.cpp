#include "calculator.h"
#include <string>
#include <exception>
#include <stack>

pugi::xml_document MathMlCalculator::doc;

MathMlCalculator::MathMlCalculator( const wchar_t* formulaPath, bool _is2D ) :
	is2D( _is2D )
{
	pugi::xml_parse_result result = doc.load_file( formulaPath );
	buildFormulas( doc );
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

					zPoints[i][j] = zFormula();
				} else {
					OperationHandler::setVar( "t", getFirstArg( i, j ) );
					OperationHandler::setVar( "u", getSecondArg( i, j ) );
					xPoints[i][j] = xFormula();
					yPoints[i][j] = yFormula();
					zPoints[i][j] = zFormula();
				}
			}
		} else {
			xPoints[i].resize( 1 );
			zPoints[i].resize( 1 );
			if( !isParametric ) {
				OperationHandler::setVar( "x", getFirstArg( i, 0 ) );
				xPoints[i][0] = getFirstArg( i, 0 );
				zPoints[i][0] = zFormula();
			} else {
				OperationHandler::setVar( "t", getFirstArg( i, 0 ) );
				xPoints[i][0] = xFormula();
				zPoints[i][0] = zFormula();
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

double MathMlCalculator::GetZ( int i, int j )
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
	return calculateMax(zPoints);
}

void MathMlCalculator::buildFormulas( const pugi::xml_node& formulaRoot )
{
	pugi::xml_node coordDefinitionNode = formulaRoot.first_child();
	buildCoordFormula(coordDefinitionNode);
	coordDefinitionNode = coordDefinitionNode.next_sibling();
	if (!coordDefinitionNode.empty()) {
		buildCoordFormula(coordDefinitionNode);
		if (!coordDefinitionNode.next_sibling().empty()) {
			buildCoordFormula(coordDefinitionNode.next_sibling());
		}
		isParametric = true;
	}
	else {
		//неявная
	}
}


void MathMlCalculator::buildCoordFormula(const pugi::xml_node& coordRoot) {
	pugi::xml_node coordIdentNode = coordRoot.first_child().next_sibling();
	std::string coordName = coordIdentNode.text().as_string();
	if (coordName == "x") {
		xFormula = [coordIdentNode]() -> double {
			return OperationHandler::getOperation(coordIdentNode.next_sibling().name()).build(coordIdentNode.next_sibling());
		};
	}
	else if (coordName == "y") {
		yFormula = [coordIdentNode]() -> double {
			return OperationHandler::getOperation(coordIdentNode.next_sibling().name()).build(coordIdentNode.next_sibling());
		};
	}
	else if (coordName == "z") {
		zFormula = [coordIdentNode]() -> double {
			return OperationHandler::getOperation(coordIdentNode.next_sibling().name()).build(coordIdentNode.next_sibling());
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

double MathMlCalculator::getFirstArg( int i, int j )
{
	return (double) (i - gridSize / 2) / 4;
}