#pragma once

#define M_PI       3,1415926535897932384626433

#include <vector> 
#include <math.h>
#include "RotationLib.h"
#include "mathCore.h"
using namespace std;

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
class GP {
public:
	// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
	GP( const MathCore& inputMCore, double inputLengthOfSection = 5, 
		const vector<double>& inputAnglesOfAxis = { -30, 40, 90 }, 
		pair<double, double> inputWindowSize = pair<double, double>(700, 700) );
	// Поворот вокруг оси Z
	void turnAroundZ( int angle = 1 );
	// Поворот вокруг оси Y
	void turnAroundY( int angle = 1 );
	// поворот вокруг оси X
	void turnAroundX( int angle = 1 );
	// Поворот вокруг произвольного вектора
	void turnRoundVector( int angle, Vector vector );
	// Смещение по вертикали
	void moveOverX( int num );
	// Смещение по горизонтали
	void moveOverY( int num );
	// изменяет масштаб
	void changeScale( int num );
	// Возвращает положение относительных точек
	vector<vector<pair<double, double>>> getRelativePoints();
	// возвращает направляющий вектор Номера осей X - 0, Y - 1, Z - 2
	pair<double, double> getAxisVector( int axisNum );
	// возвращает направляющий вектор в подстроенный под ориентацию WinApi
	pair<double, double> getAxisVectorVisual( int axisNum );
	// возвращает координаты пересечения осей в 2d
	pair<double, double> getOriginCoordinates();
	// проставляет значения начала координат
private:
	// Пересчет положения точек относительно осей
	void calculateRelativePoints();
	// создает сетку в зависимости от размера ячейки
	void generateNet();
	
	
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
	// координаты центра окна
	pair<double, double> windowSize;
	// пересчитывает координаты в зависимисти от удаления от центра окна
	MathCore mCore;
};