#include "GP.h"
#include <vector> 
#include <math.h>

#define MinLenthOfSection 5
#define MaxLengthOfSection 50


// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP(const MathCore& inputMCore, double inputLengthOfSection, const std::vector<double>& inputAnglesOfAxis, 
	std::pair<double, double> inputWindowSize )

	: mCore( inputMCore ),
	currentAngle(0, 0, 0)
{
	windowSize = inputWindowSize;
	origin.first = windowSize.first / 2;
	origin.second = windowSize.second / 2;
	lengthOfSection = inputLengthOfSection;
	anglesOfAxis.resize( 3 );
	for( int i = 0; i < 3; i++ ) {
		anglesOfAxis[i] = inputAnglesOfAxis[i];
	}
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	generateGrid();
	calculateRelativePoints();
}

void GP::generateGrid() {
	double size;
	if( windowSize.first > windowSize.second ){
		size = windowSize.first;
	}
	else {
		size = windowSize.second;
	}
	int gridSize = (int)(size/ lengthOfSection) / 2 * 2;

	points.resize(gridSize);
	relativePoints.resize(gridSize);
	for( int i = 0; i < gridSize; i++ ) {
		points[i].resize(gridSize);
		relativePoints[i].resize(gridSize);
		for( int j = 0; j < gridSize; j++ ) {
			points[i][j] = mCore.calculate( i - gridSize /2 , j - gridSize / 2 );
		}
	}
}

void GP::turnAroundZ( int angle ) {
	currentAngle.z += angle;
	Quaternion q( angle, relativeAxis[2] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnAroundY( int angle ) {
	currentAngle.y += angle;
	Quaternion q( angle, relativeAxis[1] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnAroundX( int angle ) {
	currentAngle.x += angle;
	Quaternion q( angle, relativeAxis[0] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnRoundVector( int angle, Vector vector ) {
	Quaternion q( angle, vector );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
}

std::vector<std::vector<std::pair<double, double>>> GP::getRelativePoints() {
	return relativePoints;
}

// возвращает направляющий вектор относительной( подвижной ) системы отсчета. Номера осей X - 0, Y - 1, Z - 2
std::pair<double, double> GP::getAxisVector( int axisNum ) {
	// получаем координаты неподвижной системы отсчета в 2D
	double x0 = cos( M_PI * anglesOfAxis[0] / 180 );
	double y0 = sin( M_PI * anglesOfAxis[0] / 180 );
	double x1 = cos( M_PI * anglesOfAxis[1] / 180 );
	double y1 = sin( M_PI * anglesOfAxis[1] / 180 );
	double x2 = cos( M_PI * anglesOfAxis[2] / 180 );
	double y2 = sin( M_PI * anglesOfAxis[2] / 180 );
	// пересчитываем координаты осей относительной( подвижной ) системы отсчета в 2D, используя координаты неподвижной системы
	double relX = x0 * relativeAxis[axisNum].x + x1 * relativeAxis[axisNum].y + x2 * relativeAxis[axisNum].z;
	double relY = y0 * relativeAxis[axisNum].x + y1 * relativeAxis[axisNum].y + y2 * relativeAxis[axisNum].z;
	return std::pair<double, double>( relX, relY );
}

std::pair<double, double> GP::getAxisVectorVisual( int axisNum ) {
	std::pair<double, double> axis = getAxisVector( axisNum );
	axis.second = -axis.second;
	return axis;

}

void GP::rotateToStartAngle() {
	if (currentAngle.x != 0) {
		turnAroundX(-currentAngle.x);
	}
	if (currentAngle.y != 0) {
		turnAroundY(-currentAngle.y);
	}
	if (currentAngle.z != 0) {
		turnAroundZ(-currentAngle.z);
	}
}

void GP::rotateToCurrentAngle() {
	if (currentAngle.x != 0) {
		turnAroundX(currentAngle.x);
	}
	if (currentAngle.y != 0) {
		turnAroundY(currentAngle.y);
	}
	if (currentAngle.z != 0) {
		turnAroundZ(currentAngle.z);
	}
}

void GP::moveAlongX( int num ) {
	rotateToStartAngle();

	mCore.changeWindowCoordinates( num, 0, 0 );
	generateGrid();

	rotateToCurrentAngle();
	calculateRelativePoints();
}

void GP::moveAlongY( int num ) {
	rotateToStartAngle();

	mCore.changeWindowCoordinates( 0, num, 0 );
	generateGrid();

	rotateToCurrentAngle();
	calculateRelativePoints();
}

void GP::changeScale( int num ) {
	if( lengthOfSection + num > MinLenthOfSection && lengthOfSection + num <= MaxLengthOfSection ) {
		mCore.changeScale((lengthOfSection + num) / lengthOfSection);

		lengthOfSection += num;
		generateGrid();
		calculateRelativePoints();
	}
	
}

std::pair<double, double> GP::getOriginCoordinates() {
	return origin;
}

void GP::calculateRelativePoints() {
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	double size = relativePoints.size();
	for( int i = 0; i < relativePoints.size(); i++ ) {
		for( int j = 0; j < relativePoints[i].size(); j++ ) {
			double xRel = origin.first + x.first * ( i - size / 2 ) * lengthOfSection + y.first * (j - size / 2 ) * lengthOfSection + z.first * points[i][j] * lengthOfSection;
			double yRel = origin.second + x.second * ( i - size / 2 ) * lengthOfSection + y.second * (j - size / 2 ) * lengthOfSection + z.second * points[i][j] * lengthOfSection;
			relativePoints[i][j] = std::pair<double, double>( xRel, yRel );
		}
	}
}	