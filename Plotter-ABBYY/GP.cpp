#include "GP.h"

#define MinLengthOfSection 20
#define MaxLengthOfSection 100

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP( const wchar_t* formulaPath, bool _is2D,
	double inputLengthOfSection, std::pair<double, double>& inputWindowSize ) :
	lengthOfSection( inputLengthOfSection ),
	windowSize( inputWindowSize ),
	scale( 1 )
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
std::pair<double, double> GP::getAxisVector( int axisNum ) const
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
	return std::pair<double, double>( scale * relX, scale * relY );
}

std::pair<double, double> GP::getAxisVectorVisual( int axisNum ) const
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
	//globalXShift = num;
	recalculateOrigin(num, 0, 0);
	calculateRelativePoints();
}

void GP::moveAlongY( int num )
{
	//globalYShift = num;
	recalculateOrigin(0, num, 0);
	calculateRelativePoints();
}

void GP::moveAlongZ( int num )
{
	//globalZShift = num;
	recalculateOrigin(0, 0, num);
	calculateRelativePoints();
}

void GP::changeScale( int num )
{
	if( lengthOfSection + num > MinLengthOfSection && lengthOfSection + num <= MaxLengthOfSection ) {
		scale *= (lengthOfSection + num) / lengthOfSection;

		lengthOfSection += num;

		double size = (windowSize.first > windowSize.second) ? windowSize.first : windowSize.second;
		calc.RecalculatePoints( 4 * (int) (size * 2 / lengthOfSection) );

		calculateRelativePoints();
	}
}

std::pair<double, double> GP::getOriginCoordinates()
{
	//std::pair<double, double> res = origin;
	/*std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	res.first -= x.first * globalXShift * lengthOfSection + y.first * globalYShift * lengthOfSection +
		z.first * globalZShift * lengthOfSection;
	res.second -= x.second * globalXShift * lengthOfSection + y.second * globalYShift * lengthOfSection +
		z.second * globalZShift * lengthOfSection;*/
	return origin;
}


void GP::changeSize(double x, double y) {
	if (windowSize.first != x || windowSize.second != y) {
		windowSize.first = x;
		windowSize.second = y;
		origin.first = windowSize.first;
		origin.second = windowSize.second;
		calculateRelativePoints();
	}
	/*
	origin.first = x;
	origin.second = y;
	calculateRelativePoints();*/
	/*globalXShift = 0;
	globalYShift = 0;
	globalZShift = 0;*/
}

void GP::recalculateOrigin(int xShift, int yShift, int zShift) {
	std::pair<double, double> x = getAxisVectorVisual(0);
	std::pair<double, double> y = getAxisVectorVisual(1);
	std::pair<double, double> z = getAxisVectorVisual(2);
	origin.first += (x.first * xShift  + y.first * yShift +
		z.first * zShift ) * lengthOfSection;
	origin.second += (x.second * xShift + y.second * yShift +
		z.second * zShift) * lengthOfSection;
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
				double xRel = origin.first + ( x.first * calc.GetX( i, j )) * lengthOfSection +
					(y.first * calc.GetY( i, j )) * lengthOfSection +
					z.first * (calc.GetZ( i, j )) * lengthOfSection;
				double yRel = origin.second + (x.second * calc.GetX(i, j)) * lengthOfSection +
					y.second * (calc.GetY( i, j )) * lengthOfSection +
					z.second * (calc.GetZ( i, j )) * lengthOfSection;
				relativePoints[i][j].first = xRel;
				relativePoints[i][j].second = yRel;
			}
		} else {
			relativePoints[i].resize( 1 );
			double xRel = origin.first + ( x.first * (calc.GetX( i, 0 )) * lengthOfSection +
				z.first * (calc.GetZ( i, 0 )) * lengthOfSection );
			double yRel = origin.second + ( x.second * (calc.GetX( i, 0 )) * lengthOfSection +
				z.second * (calc.GetZ( i, 0 )) * lengthOfSection );
			relativePoints[i][0].first = xRel;
			relativePoints[i][0].second = yRel;
		}
	}
}


std::pair<double, double> GP::getProjection(double value, int axisNum) const
{
	std::pair<double, double> axis = getAxisVectorVisual(axisNum);
	return std::make_pair(origin.first + axis.first * value * lengthOfSection,
		origin.second + axis.second * value* lengthOfSection);
}

std::pair<double, double> GP::getMaxRelativePoint(std::pair<double, double>& axis, int top, int bottom, int left, int right) {
	double maxX;
	double maxY;
	if (fabs(axis.first) > fabs(axis.second)) {
		maxX = (axis.first > 0) ? right : left;
		maxY = origin.second + (maxX - origin.first) / axis.first * axis.second;
	}
	else {
		maxY = (axis.second > 0) ? bottom : top;
		maxX = origin.first + (maxY - origin.second) / axis.second * axis.first;
	}
	return std::make_pair(maxX, maxY);
}


double GP::getAxisMax(const std::pair<double, double>& relativeMaxPoint, const std::pair<double, double>& axis) const{
	return  (axis.second == 0)? (relativeMaxPoint.first - origin.first) / (lengthOfSection * axis.first)
		: (relativeMaxPoint.second - origin.second) / (lengthOfSection * axis.second);
}

std::vector< std::vector<double> > GP::getZcoordinates() {
	std::vector< std::vector<double> > zCoordinates;
	zCoordinates.resize(calc.GetGridSize());
	for (int i = 0; i < zCoordinates.size(); ++i) {
		zCoordinates[i].resize(calc.GetGridSize());
		for (int j = 0; j < zCoordinates[i].size(); ++j) {
			zCoordinates[i][j] = calc.GetZ(i, j);
		}
	}
	return zCoordinates;
}

std::pair<double, double> GP::getRelativePointWithXYZ( int i, int j, double zValue ) {
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	double xRel = origin.first + ( x.first * (calc.GetX( i, j )) * lengthOfSection +
					y.first * (calc.GetY( i, j )) * lengthOfSection +
					z.first * (zValue) * lengthOfSection );
	double yRel = origin.second + ( x.second * (calc.GetX( i, j ) ) * lengthOfSection +
					y.second * (calc.GetY( i, j )) * lengthOfSection +
					z.second * (zValue) * lengthOfSection );
	return std::make_pair( xRel, yRel );
}

int GP::getGridSize() {
	return calc.GetGridSize();
}