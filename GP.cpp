#include "GP.h""
#include <vector> 
#include <math.h>
using namespace std;

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP( const vector<vector<double>>& inputPoints, double inputLengthOfSection, const vector<double>& inputAnglesOfAxis ) {
	points.resize( inputPoints.size() );
	relativePoints.resize( inputPoints.size() );
	for( int i = 0; i < inputPoints.size(); i++ ) {
		points[i].resize( inputPoints[i].size() );
		relativePoints[i].resize( inputPoints[i].size() );
		for( int j = 0; j < inputPoints[i].size(); j++ ) {
			points[i][j] = inputPoints[i][j];
		}
	}
	anglesOfAxis.resize( 3 );
	for( int i = 0; i < 3; i++ ) {
		anglesOfAxis[i] = inputAnglesOfAxis[i];
	}
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	lengthOfSection = inputLengthOfSection;
	calculateRelativePoints();
}

GP::GP() {
	// По умолчанию инициализируем сеткой 20 на 20
	points.resize( 20 );
	relativePoints.resize( 20 );
	for( int i = 0; i < 20; i++ ) {
		points[i].resize( 20 );
		relativePoints[i].resize( 20 );
		for( int j = 0; j < 20; j++ ) {
			points[i][j] = 5;
		}
	}
	anglesOfAxis.resize( 3 );
	anglesOfAxis = { 0, 45, 90 };
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	lengthOfSection = 1;
	calculateRelativePoints();
}

void GP::turnFromTheTopToDown( int angle ) {
	Quaternion q( angle, -1, 0, 0 );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnClockwise( int angle ) {
	Quaternion q( angle, 0, -1, 0 );
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



pair<double, double> GP::getOriginCoordinates() {
	origin.first = 0;
	origin.second = 0;
	return origin;
}

void GP::calculateRelativePoints() {
	pair<double, double> x = getAxisVector( 0 );
	pair<double, double> y = getAxisVector( 1 );
	pair<double, double> z = getAxisVector( 2 );
	for( int i = 0; i < relativePoints.size(); i++ ) {
		for( int j = 0; j < relativePoints[i].size(); j++ ) {
			double xRel = x.first * i * lengthOfSection + y.first * j * lengthOfSection + z.first * points[i][j];
			double yRel = x.second * i * lengthOfSection + y.second * j * lengthOfSection + z.second * points[i][j];
			relativePoints[i][j] = pair<double, double>( xRel, yRel );
		}
	}
}	