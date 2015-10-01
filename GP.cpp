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
	lengthOfSection = inputLengthOfSection;
	CalculateRelativePoints();
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
	lengthOfSection = 1;
	CalculateRelativePoints();
}

void GP::TurnFromTheTopToDown( int angle ) {
}

void GP::TurnClockwise() {
}

vector<vector<pair<double, double>>> GP::GetRelativePoints() {
	return relativePoints;
}

// возвращает направляющий вектор Номера осей X - 0, Y - 1, Z - 2
pair<double, double> GP::getAxisVector( int axisNum ) {
	double x = cos( M_PI * anglesOfAxis[axisNum] / 180 );
	double y = sin( M_PI * anglesOfAxis[axisNum] / 180 );
	return pair<double, double>( x, y );
}

pair<double, double> GP::getOriginCoordinates() {
	origin.first = 0;
	origin.second = 0;
	return origin;
}

void GP::CalculateRelativePoints() {
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