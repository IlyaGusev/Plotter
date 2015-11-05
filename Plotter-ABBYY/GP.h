#pragma once

#include <vector> 
#include <math.h>
#include "RotationLib.h"
#include "calculator.h"


// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
class GP {
public:
	// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
	GP( const wchar_t* formulaPath,
		bool is2D = false, bool isImplisit = false, double inputLengthOfSection = 5,
		std::pair<double, double>& inputWindowSize = std::pair<double, double>( 700, 700 ) );

	// Повороты при нажатии на соответствующие кнопки на клавиатуре
	void turnLeft();
	void turnRight();
	void turnUp();
	void turnDown();

	// Поворот вокруг произвольного вектора
	void turnRoundVector( int angle, Vector vector );
	// Смещение по вертикали
	void moveAlongX( int num );
	// Смещение по горизонтали
	void moveAlongY( int num );
	// Смещение по оси Z
	void moveAlongZ( int num );
	// изменяет масштаб
	void changeScale( int num );
	// Возвращает положение относительных точек
	std::vector<std::vector< std::vector< std::pair<double, double> >>> getRelativePoints();
	// возвращает направляющий вектор Номера осей X - 0, Y - 1, Z - 2
	std::pair<double, double> getAxisVector( int axisNum ) const;
	// возвращает направляющий вектор в подстроенный под ориентацию WinApi
	std::pair<double, double> getAxisVectorVisual( int axisNum ) const;
	// возвращает координаты пересечения осей в 2d
	std::pair<double, double> getOriginCoordinates();
	// возвращает значения Z для пары XY
	std::vector<std::vector<std::vector<double>>> getZcoordinates();
	// возвращает точку на экране, которая соответствует точке на графике с координатой Z и узлу сетки i j 
	std::pair<double, double> getRelativePointWithXYZ( int i, int j, double z );
	// возвращает размер сетки
	int getGridSize();
	
	std::pair<double, double> getXProjection(double x) const;
	std::pair<double, double> getYProjection(double y) const;
	std::pair<double, double> getZProjection(double z) const;

	double getXMax() const;
	double getYMax() const;
	double getZMax() const;

private:
	// Пересчет положения точек относительно осей
	void calculateRelativePoints();

	void rotateToStartAngle();
	void rotateToCurrentAngle();

	void turnAroundAxis( int axisNumber, int angle = 1 );

	// углы, под которыми расположены главные( неподвижные ) оси на 2D по отношению к стандартной оси X
	std::vector< double > anglesOfAxis;
	// относительные( подвижные ) оси вращающейся фигуры или графика функции
	std::vector< Vector > relativeAxis;
	std::vector< Vector > prevRelativeAxis;
	// относительные точки уже в 2D, в качестве индексов изначальная сетка X, Y 
	std::vector < std::vector < std::vector< std::pair<double, double > > > > relativePoints;
	// координаты пересечения осей в 2D
	std::pair<double, double> origin;

	// Считает значения функции в конкретных точках
	MathMlCalculator calc;

	// длина отрезка сетки
	double lengthOfSection;

	// координаты центра окна
	std::pair<double, double> windowSize;

	// Смещения и увеличение графика
	double globalXShift;
	double globalYShift;
	double globalZShift;
	double scale;
};