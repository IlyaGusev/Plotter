#include "calculator.h"
#include <string>
#include <exception>
#include <stack>

pugi::xml_document MathMlCalculator::doc;

MathMlCalculator::MathMlCalculator( const wchar_t* formulaPath, bool _is2D ) :
	is2D( _is2D )
{
	pugi::xml_parse_result result = doc.load_file( formulaPath );
	buildFormula( doc );
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

double MathMlCalculator::getFirstArg( int i, int j )
{
	return (double) (i - gridSize / 2) / 4;
}