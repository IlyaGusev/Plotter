#include "GP.h""
#include <vector> 
#include <math.h>
using namespace std;

// Graph in Points
// ƒанный класс предназначен дл€ поточечного представлени€ графика в зависимости от положени€ осей 
	// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
	GP::GP( const vector<vector<double>>& inputPoints, double inputLengthOfSection, const vector<double>& inputAnglesOfAxis ) {
		points.resize( inputPoints.size() );
		relativePoints.resize( inputPoints.size() );
		for( int i = 0; i < inputPoints.size(); i++ ) {
			points[i].resize( inputPoints[i].size() );
			relativePoints.resize( inputPoints[i].size() );
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
		// ѕо умолчанию инициализируем сеткой 20 на 20
		points.resize( 20 );
		relativePoints.resize( 20 );
		for( int i = 0; i < 20; i++ ) {
			points[i].resize( 20 );
			relativePoints.resize( 20 );
			for( int j = 0; j < 20; j++ ) {
				points[i][j] = 5;
			}
		}
		anglesOfAxis.resize( 3 );
		anglesOfAxis = { 0, 45, 90 };
		lengthOfSection = 1;
	}
	void GP::TurnFromTheTopToDown( int angle ) {

	}
	void GP::TurnClockwise() {}
	vector<vector<pair<double, double>>> GP::GetRelativePoints() {
		return relativePoints;
	}
	// возвращвет направл€ющий вектор Ќомера осей X - 0, Y - 1, Z - 2
	pair<double, double> GP::getAxisVector( int axisNum ) {
		double x = cos( M_PI * anglesOfAxis[axisNum] / 180 );
		double y = sin( M_PI * anglesOfAxis[axisNum] / 180 );
		return pair<double, double>( x, y );
	}


	void GP::CalculateRelativePoints() {
		pair<double, double> x = getAxisVector( 0 );
		pair<double, double> y = getAxisVector( 0 );
		pair<double, double> z = getAxisVector( 0 );
		for( int i = 0; i < relativePoints.size(); i++ ) {
			for( int j = 0; j < relativePoints[i].size(); j++ ) {
				double xRel = x.first * i * lengthOfSection + y.first * j * lengthOfSection + z.first * points[i][j];
				double yRel = x.second * i * lengthOfSection + y.second * j * lengthOfSection + z.second * points[i][j];
				relativePoints[i][j] = pair<double, double>( xRel, yRel );
			}
		}
	}


	