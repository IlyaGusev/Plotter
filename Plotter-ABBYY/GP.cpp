#include "GP.h"

#define MinLengthOfSection 20
#define MaxLengthOfSection 100

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP( const wchar_t* formulaPath, bool _is2D,
	double inputLengthOfSection, std::pair<double, double>& inputWindowSize ) :
	lengthOfSection( inputLengthOfSection ),
	windowSize( inputWindowSize )
{
	calc = MathMlCalculator( formulaPath, _is2D );

	origin.first = windowSize.first / 2;
	origin.second = windowSize.second / 2;
	anglesOfAxis = calc.Is2D() ? std::vector<double>{0, 90, 90} : std::vector<double>{ -30, 40, 90 };
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	prevRelativeAxis = relativeAxis;

	double size = (windowSize.first > windowSize.second) ? windowSize.first : windowSize.second;
	calc.RecalculatePoints( 4 * (int) (size / lengthOfSection) );

	calculateRelativePoints();
}

//void GP::turnAroundZ( int angle ) {
//	Quaternion q( angle, relativeAxis[2] );
//	for( int i = 0; i < 3; i++ ) {
//		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
//	}
//	calculateRelativePoints();
//}
//
//void GP::turnAroundY( int angle ) {
//	Quaternion q( angle, relativeAxis[1] );
//	for( int i = 0; i < 3; i++ ) {
//		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
//	}
//	calculateRelativePoints();
//}

void GP::turnAroundAxis( int axisNumber, int angle )
{
	Quaternion q( angle, relativeAxis[axisNumber] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnLeft()
{
	if( calc.Is2D() ) {
		moveAlongX( -1 );
	} else {
		turnAroundAxis( 2, -1 );
	}
}
void GP::turnRight()
{
	if( calc.Is2D() ) {
		moveAlongX( 1 );
	} else {
		turnAroundAxis( 2, 1 );
	}
}
void GP::turnUp()
{
	if( calc.Is2D() ) {
		moveAlongZ( 1 );
	} else {
		turnAroundAxis( 1, 1 );
	}
}
void GP::turnDown()
{
	if( calc.Is2D() ) {
		moveAlongZ( -1 );
	} else {
		turnAroundAxis( 1, -1 );
	}
}

void GP::turnRoundVector( int angle, Vector vector )
{
	Quaternion q( angle, vector );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
}

std::vector<std::vector<std::pair<double, double>>> GP::getRelativePoints()
{
	return relativePoints;
}

// возвращает направляющий вектор относительной( подвижной ) системы отсчета. Номера осей X - 0, Y - 1, Z - 2
std::pair<double, double> GP::getAxisVector( int axisNum )
{
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
	return std::pair<double, double>( calc.Scale() * relX, calc.Scale() * relY );
}

std::pair<double, double> GP::getAxisVectorVisual( int axisNum )
{
	std::pair<double, double> axis = getAxisVector( axisNum );
	axis.second = -axis.second;
	return axis;

}

void GP::rotateToStartAngle()
{
	prevRelativeAxis = relativeAxis;
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
}

void GP::rotateToCurrentAngle()
{
	relativeAxis = prevRelativeAxis;
}

void GP::moveAlongX( int num )
{
	calc.GlobalXShift() += num;
	calculateRelativePoints();
}

void GP::moveAlongY( int num )
{
	calc.GlobalYShift() += num;
	calculateRelativePoints();
}

void GP::moveAlongZ( int num )
{
	calc.GlobalZShift() += num;
	calculateRelativePoints();
}

void GP::changeScale( int num )
{
	if( lengthOfSection + num > MinLengthOfSection && lengthOfSection + num <= MaxLengthOfSection ) {
		calc.Scale() *= (lengthOfSection + num) / lengthOfSection;

		lengthOfSection += num;

		double size = (windowSize.first > windowSize.second) ? windowSize.first : windowSize.second;
		calc.RecalculatePoints( 4 * (int) (size / lengthOfSection) );

		calculateRelativePoints();
	}
}

std::pair<double, double> GP::getOriginCoordinates()
{
	std::pair<double, double> res = origin;
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	res.first -= x.first * calc.GlobalXShift() * lengthOfSection + y.first * calc.GlobalYShift() * lengthOfSection +
		z.first * calc.GlobalZShift() * lengthOfSection;
	res.second -= x.second * calc.GlobalXShift() * lengthOfSection + y.second * calc.GlobalYShift() * lengthOfSection +
		z.second * calc.GlobalZShift() * lengthOfSection;
	return res;
}

void GP::calculateRelativePoints()
{
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	relativePoints.resize( calc.GetGridSize() );
	for( int i = 0; i < relativePoints.size(); i++ ) {
		if( !calc.Is2D() ) {
			relativePoints[i].resize( calc.GetGridSize() );
			for( int j = 0; j < relativePoints[i].size(); j++ ) {
				double xRel = origin.first + x.first * (calc.GetX( i, j ) - calc.GlobalXShift()) * lengthOfSection +
					y.first * (calc.GetY( i, j ) - calc.GlobalYShift()) * lengthOfSection +
					z.first * (calc.GetZ( i, j ) - calc.GlobalZShift()) * lengthOfSection;
				double yRel = origin.second + x.second * (calc.GetX( i, j ) - calc.GlobalXShift()) * lengthOfSection +
					y.second * (calc.GetY( i, j ) - calc.GlobalYShift()) * lengthOfSection +
					z.second * (calc.GetZ( i, j ) - calc.GlobalZShift()) * lengthOfSection;
				relativePoints[i][j] = std::pair<double, double>( xRel, yRel );
			}
		} else {
			relativePoints[i].resize( 1 );
			double xRel = origin.first + x.first * (calc.GetX( i, 0 ) - calc.GlobalXShift()) * lengthOfSection +
				z.first * (calc.GetZ( i, 0 ) - calc.GlobalZShift()) * lengthOfSection;
			double yRel = origin.second + x.second * (calc.GetX( i, 0 ) - calc.GlobalXShift()) * lengthOfSection +
				z.second * (calc.GetZ( i, 0 ) - calc.GlobalZShift()) * lengthOfSection;
			relativePoints[i][0] = std::pair<double, double>( xRel, yRel );
		}
	}
}