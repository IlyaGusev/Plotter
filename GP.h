#pragma once

#define M_PI       3.14159265358979323846

#include <vector> 
#include <math.h>
#include "RotationLib.h"
using namespace std;

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
class GP {
public:
	// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
	GP( const vector<vector<double>>& inputPoints, double inputLengthOfSection = 1, const vector<double>& inputAnglesOfAxis = { 0, 45, 90 } );
	GP();
	void turnFromTheTopToDown( int angle = 1 );
	void turnClockwise( int angle = 1 );
	void turnRoundVector( int angle, Vector vector );
	// Возвращает положение относительных точек
	vector<vector<pair<double, double>>> GetRelativePoints();
	// возвращает направляющий вектор Номера осей X - 0, Y - 1, Z - 2
	pair<double, double> getAxisVector( int axisNum );
	// возвращает координаты пересечения осей в 2d
	pair<double, double> getOriginCoordinates();
	
private:
	// Пересчет положения точек относительно осей
	void calculateRelativePoints();
	
	
	
	// точки изначальные, в качестве индексов - узлы сетки XY
	vector<vector<double>> points;
	// углы под которыми расположены глаавные( неподвижные ) оси на 2D по отношению к стандартной оси X
	vector< double > anglesOfAxis;
	// относительные( подвижные ) оси вращающейся фигуры или графика функции
	vector< Vector > relativeAxis;
	// относительные точки уже в 2D, в качестве индексов изначальная сетка X, Y 
	vector<vector<pair<double, double>>> relativePoints;
	// длина отрезка сетки
	double lengthOfSection;
	// координаты пересечения осей в 2D
	pair<double, double> origin;
};