#pragma once

#define M_PI       3.14159265358979323846

#include <vector> 
#include <math.h>
using namespace std;

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
class GP {
public:
	// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
	GP( const vector<vector<double>>& inputPoints, double inputLengthOfSection = 1, const vector<double>& inputAnglesOfAxis = { 0, 45, 90 } );
	GP();
	void TurnFromTheTopToDown( int angle = 1 );
	void TurnClockwise();
	// Возвращает положение относительных точек
	vector<vector<pair<double, double>>> GetRelativePoints();
	// возвращвет направляющий вектор Номера осей X - 0, Y - 1, Z - 2
	pair<double, double> getAxisVector( int axisNum );
	
private:
	// Пересчет положения точек относительно осей
	void CalculateRelativePoints();
	// точки изначальные, в качестве индексов - узлы сетки XY
	vector<vector<double>> points;
	// углы под которыми расположены оси по отношению к стандартной оси X
	vector< double > anglesOfAxis;
	// относительные точки уже в 2d, в качестве индексов изначальная сетка X, Y 
	vector<vector<pair<double, double>>> relativePoints;
	// длина отрезка сетки
	double lengthOfSection;
};