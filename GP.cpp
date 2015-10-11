#include "GP.h""
#include <vector> 
#include <math.h>
using namespace std;

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP( MathCore inputMCore, double inputLengthOfSection, const vector<double>& inputAnglesOfAxis, pair<double, double> inputWindowSize ) {
	windowSize = inputWindowSize;
	origin.first = windowSize.first / 2;
	origin.second = windowSize.second / 2;
	mCore = inputMCore;
	lengthOfSection = inputLengthOfSection;
	anglesOfAxis.resize( 3 );
	for( int i = 0; i < 3; i++ ) {
		anglesOfAxis[i] = inputAnglesOfAxis[i];
	}
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	generateNet();
	calculateRelativePoints();
}

GP::GP() {
	// По умолчанию инициализируем сеткой 40 на 40
	windowSize.first = 700;
	windowSize.second = 700;
	origin.first = 350;
	origin.second = 350;
	anglesOfAxis.resize( 3 );
	anglesOfAxis = { -30, 35, 90 };
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	lengthOfSection = 20;
	generateNet();
	calculateRelativePoints();
}

void GP::generateNet() {
	double size;
	if( windowSize.first > windowSize.second ){
		size = windowSize.first;
	}
	else {
		size = windowSize.second;
	}
	int netSize = (int)(size / lengthOfSection) / 2 * 2;

	points.resize( netSize );
	relativePoints.resize( netSize );
	for( int i = 0; i < netSize; i++ ) {
		points[i].resize( netSize );
		relativePoints[i].resize( netSize );
		for( int j = 0; j < netSize; j++ ) {
			//points[i][j] = mCore.calculate( origin.first - windowSize.first / 2, origin.second - windowSize.second / 2 );
			points[i][j] = 5;
		}
	}
}

void GP::turnAroundZ( int angle ) {
	Quaternion q( angle, relativeAxis[2] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnAroundY( int angle ) {
	Quaternion q( angle, relativeAxis[1] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnAroundX( int angle ) {
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

vector<vector<pair<double, double>>> GP::GetRelativePoints() {
	return relativePoints;
}

// возвращает направляющий вектор относительной( подвижной ) системы отсчета. Номера осей X - 0, Y - 1, Z - 2
pair<double, double> GP::getAxisVector( int axisNum ) {
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
	return pair<double, double>( relX, relY );
}

pair<double, double> GP::getAxisVectorVisual( int axisNum ) {
	pair<double, double> axis = getAxisVector( axisNum );
	axis.second = -axis.second;
	return axis;

}

void GP::moveOverX( int num ) {
	origin.first += num*lengthOfSection * cos( M_PI * anglesOfAxis[0] / 180 );
	origin.second += num*lengthOfSection *  sin( M_PI * anglesOfAxis[0] / 180 );
	mCore.changeWindowCoordinates( num, 0, 0 );
}
void GP::moveOverY( int num ) {
	origin.first += num*lengthOfSection * cos( M_PI * anglesOfAxis[1] / 180 );
	origin.second += num*lengthOfSection * sin( M_PI * anglesOfAxis[1] / 180 );
	mCore.changeWindowCoordinates( 0, num, 0 );
}

pair<double, double> GP::getOriginCoordinates() {
	return origin;
}

void GP::calculateRelativePoints() {
	pair<double, double> x = getAxisVectorVisual( 0 );
	pair<double, double> y = getAxisVectorVisual( 1 );
	pair<double, double> z = getAxisVectorVisual( 2 );
	double size = relativePoints.size();
	for( int i = 0; i < relativePoints.size(); i++ ) {
		for( int j = 0; j < relativePoints[i].size(); j++ ) {
			double xRel = origin.first + x.first * ( i - size / 2 ) * lengthOfSection + y.first * (j - size / 2 ) * lengthOfSection + z.first * points[i][j] * lengthOfSection;
			double yRel = origin.second + x.second * ( i - size / 2 ) * lengthOfSection + y.second * (j - size / 2 ) * lengthOfSection + z.second * points[i][j] * lengthOfSection;
			relativePoints[i][j] = pair<double, double>( xRel, yRel );
		}
	}
}	