#include "GP.h"

#define MinLengthOfSection 20
#define MaxLengthOfSection 100

// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP( const wchar_t* formulaPath, bool is2D /*= false*/,
	double inputLengthOfSection /*= 5*/, std::pair<double, double>& inputWindowSize /*= std::pair<double, double>( 700, 700 ) */ ) :
	lengthOfSection( inputLengthOfSection ),
	windowSize( inputWindowSize ),
	scale( 1 ),
	globalXShift( 0 ),
	globalYShift( 0 ),
	globalZShift( 0 )
{
	calc = MathMlCalculator( formulaPath, is2D );

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
	calculateZcoordinates();

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

std::vector<std::vector<std::vector<std::pair<double, double>>>> GP::getRelativePoints()
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
	globalXShift += num;
	calculateRelativePoints();
}

void GP::moveAlongY( int num )
{
	globalYShift += num;
	calculateRelativePoints();
}

void GP::moveAlongZ( int num )
{
	globalZShift += num;
	calculateRelativePoints();
}

void GP::changeScale( int num )
{
	if( lengthOfSection + num > MinLengthOfSection && lengthOfSection + num <= MaxLengthOfSection ) {
		scale *= (lengthOfSection + num) / lengthOfSection;

		lengthOfSection += num;

		double size = (windowSize.first > windowSize.second) ? windowSize.first : windowSize.second;
		//calc.RecalculatePoints( 4 * (int) (size / lengthOfSection) );

		calculateRelativePoints();
	}
}

std::pair<double, double> GP::getOriginCoordinates()
{
	std::pair<double, double> res = origin;
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	res.first -= lengthOfSection * ( x.first * globalXShift + y.first * globalYShift + z.first * globalZShift );
	res.second -= lengthOfSection * ( x.second * globalXShift + y.second * globalYShift + z.second * globalZShift );
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
				auto z_ij = calc.GetZ( i, j );
				relativePoints[i][j].resize( z_ij.size() );
				for( size_t k = 0; k < z_ij.size(); ++k ) {
					double xRel = origin.first + scale *(x.first * (calc.GetX( i, j ) - globalXShift) * lengthOfSection +
						y.first * (calc.GetY( i, j ) - globalYShift) * lengthOfSection +
						z.first * (z_ij[k] - globalZShift) * lengthOfSection);
					double yRel = origin.second + scale * (x.second * (calc.GetX( i, j ) - globalXShift) * lengthOfSection +
						y.second * (calc.GetY( i, j ) - globalYShift) * lengthOfSection +
						z.second * (z_ij[k] - globalZShift) * lengthOfSection);

					relativePoints[i][j][k] = std::pair<double, double>( xRel, yRel );
				}
			}
		} else {
			auto z_ij = calc.GetZ( i, 0 );
			relativePoints[i].resize( 1 );
			relativePoints[i][0].resize( z_ij.size() );
			for( size_t k = 0; k < z_ij.size(); ++k ) {
				double xRel = origin.first + scale * (x.first * (calc.GetX( i, 0 ) - globalXShift) * lengthOfSection +
					z.first * (z_ij[k] - globalZShift) * lengthOfSection);
				double yRel = origin.second + scale * (x.second * (calc.GetX( i, 0 ) - globalXShift) * lengthOfSection +
					z.second * (z_ij[k] - globalZShift) * lengthOfSection);

				relativePoints[i][0][k] = std::pair<double, double>( xRel, yRel );
			}
		}
	}
}

std::pair<double, double> GP::getXProjection(double x) const
{
	std::pair<double, double> xAxis = getAxisVectorVisual(0);
	return std::make_pair(origin.first + xAxis.first * (x - globalXShift) * lengthOfSection,
		origin.second + xAxis.second * (x - globalXShift) * lengthOfSection);
}

std::pair<double, double> GP::getYProjection(double y) const
{
	std::pair<double, double> yAxis = getAxisVectorVisual(1);
	return std::make_pair(origin.first + yAxis.first * (y - globalYShift) * lengthOfSection,
		origin.second + yAxis.second * (y - globalYShift) * lengthOfSection);
}

std::pair<double, double> GP::getZProjection(double z) const
{
	std::pair<double, double> zAxis = getAxisVectorVisual(2);
	return std::make_pair(origin.first + zAxis.first * (z - globalZShift) * lengthOfSection,
		origin.second + zAxis.second * (z - globalZShift) * lengthOfSection);
}

double GP::getXMax() const {
	return calc.getXMax();
}

double GP::getYMax() const {
	return calc.getYMax();
}

double GP::getZMax() const {
	return calc.getZMax();
}

std::vector<std::vector<std::vector<double>>> GP::getZcoordinates()
{
	return zCoordinates;
}

void GP::calculateZcoordinates()
{
	zCoordinates.resize( calc.GetGridSize() );
	for( size_t i = 0; i < calc.GetGridSize(); ++i ) {
		zCoordinates[i].resize( calc.GetGridSize() );
		for( size_t j = 0; j < calc.GetGridSize(); ++j ) {
			auto z_ij = calc.GetZ( i, j );
			zCoordinates[i][j].resize( z_ij.size() );
			for( size_t k = 0; k < z_ij.size(); ++k ) {
				zCoordinates[i][j][k] = z_ij[k] - globalZShift;
			}
		}
	}
}

std::pair<double, double> GP::getRelativePointWithXYZ( int i, int j, double zValue )
{
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	double xRel = origin.first + scale * (x.first * (calc.GetX( i, j ) - globalXShift) * lengthOfSection +
		y.first * (calc.GetY( i, j ) - globalYShift) * lengthOfSection +
		z.first * (zValue - globalZShift) * lengthOfSection);
	double yRel = origin.second + scale * (x.second * (calc.GetX( i, j ) - globalXShift) * lengthOfSection +
		y.second * (calc.GetY( i, j ) - globalYShift) * lengthOfSection +
		z.second * (zValue - globalZShift) * lengthOfSection);
	return std::make_pair( xRel, yRel );
}

int GP::getGridSize() {
	return calc.GetGridSize();
}