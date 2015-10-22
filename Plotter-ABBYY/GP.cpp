#include "GP.h"

#define MinLengthOfSection 5
#define MaxLengthOfSection 200


// Graph in Points
// Данный класс предназначен для поточечного представления графика в зависимости от положения осей 
// получает на вход точки, длину стороны сетки, и углы под которыми расположены оси по отношению к стандартному положению оси X(----->)
GP::GP( const MathCore& inputMCore, const std::vector<double>& inputAnglesOfAxis,
	double inputLengthOfSection, std::pair<double, double>& inputWindowSize ) :
	mCore( inputMCore )
{
	windowSize = inputWindowSize;
	origin.first = windowSize.first / 2;
	origin.second = windowSize.second / 2;
	lengthOfSection = inputLengthOfSection;
	anglesOfAxis = inputAnglesOfAxis;
	relativeAxis.resize( 3 );
	relativeAxis[0] = Vector( 1, 0, 0 );
	relativeAxis[1] = Vector( 0, 1, 0 );
	relativeAxis[2] = Vector( 0, 0, 1 );
	prevRelativeAxis = relativeAxis;
	generateGrid();
	calculateRelativePoints();
}

GP::GP(const MathCore& inputMCore, double inputLengthOfSection, std::pair<double, double>& inputWindowSize ) : 
GP( inputMCore, inputMCore.Is2D() ? std::vector<double>{0, 90, 90} : std::vector<double>{ -30, 40, 90 }, inputLengthOfSection, inputWindowSize )
{}

void GP::generateGrid() {
	double size;
	if( windowSize.first > windowSize.second ){
		size = windowSize.first;
	}
	else {
		size = windowSize.second;
	}
	int gridSize = 4 * (int) (size / lengthOfSection);

	points.resize(gridSize);
	relativePoints.resize(gridSize);
	for( int i = 0; i < gridSize; i++ ) {
		if( !mCore.Is2D() ) {
			points[i].resize( gridSize );
			relativePoints[i].resize( gridSize );
			for( int j = 0; j < gridSize; j++ ) {
				points[i][j] = mCore.calculate( (double) (i - gridSize / 2) / 4, (double) (j - gridSize / 2) / 4 );
			}
		} else {
			relativePoints[i].resize( 1 );
			points[i].resize( 1 );
			points[i][0] = mCore.calculate( (double) (i - gridSize / 2) / 4, 0 );
		}
	}
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

void GP::turnAroundAxis( int axisNumber, int angle ) {
	Quaternion q( angle, relativeAxis[axisNumber] );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
	calculateRelativePoints();
}

void GP::turnLeft()
{
	if( mCore.Is2D() ) {
		moveAlongX( -1 );
	} else {
		turnAroundAxis( 1, -1 );
	}
}
void GP::turnRight()
{
	if( mCore.Is2D() ) {
		moveAlongX( 1 );
	} else {
		turnAroundAxis( 1, 1 );
	}
}
void GP::turnUp()
{
	if( mCore.Is2D() ) {
		moveAlongZ( 1 );
	} else {
		turnAroundAxis( 2 );
	}
}
void GP::turnDown()
{
	if( mCore.Is2D() ) {
		moveAlongZ( -1 );
	} else {
		turnAroundAxis( 2, -1 );
	}
}

void GP::turnRoundVector( int angle, Vector vector ) {
	Quaternion q( angle, vector );
	for( int i = 0; i < 3; i++ ) {
		relativeAxis[i] = q.makeRotation( relativeAxis[i] );
	}
}

std::vector<std::vector<std::pair<double, double>>> GP::getRelativePoints() {
	return relativePoints;
}

// возвращает направляющий вектор относительной( подвижной ) системы отсчета. Номера осей X - 0, Y - 1, Z - 2
std::pair<double, double> GP::getAxisVector( int axisNum ) {
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
	return std::pair<double, double>( mCore.scale * relX, mCore.scale * relY );
}

std::pair<double, double> GP::getAxisVectorVisual( int axisNum ) {
	std::pair<double, double> axis = getAxisVector( axisNum );
	axis.second = -axis.second;
	return axis;

}

void GP::rotateToStartAngle() {
	prevRelativeAxis = relativeAxis;
	relativeAxis[0] = Vector(1, 0, 0);
	relativeAxis[1] = Vector(0, 1, 0);
	relativeAxis[2] = Vector(0, 0, 1);
}

void GP::rotateToCurrentAngle() {
	relativeAxis = prevRelativeAxis;
}

void GP::moveAlongX( int num ) {
	rotateToStartAngle();

	mCore.changeWindowCoordinates( num, 0, 0 );
	generateGrid();

	rotateToCurrentAngle();
	calculateRelativePoints();
}

void GP::moveAlongY( int num ) {
	rotateToStartAngle();

	mCore.changeWindowCoordinates( 0, num, 0 );
	generateGrid();

	rotateToCurrentAngle();
	calculateRelativePoints();
}

void GP::moveAlongZ( int num )
{
	rotateToStartAngle();

	mCore.changeWindowCoordinates( 0, 0, num );
	generateGrid();

	rotateToCurrentAngle();
	calculateRelativePoints();
}

void GP::changeScale( int num ) {
	if( lengthOfSection + num > MinLengthOfSection && lengthOfSection + num <= MaxLengthOfSection ) {
		mCore.changeScale((lengthOfSection + num) / lengthOfSection);

		lengthOfSection += num;
		generateGrid();
		calculateRelativePoints();
	}
	
}

std::pair<double, double> GP::getOriginCoordinates() {
	return origin;
}

void GP::calculateRelativePoints() {
	std::pair<double, double> x = getAxisVectorVisual( 0 );
	std::pair<double, double> y = getAxisVectorVisual( 1 );
	std::pair<double, double> z = getAxisVectorVisual( 2 );
	double size = relativePoints.size();
	for( int i = 0; i < relativePoints.size(); i++ ) {
		if( !mCore.Is2D() ) {
			for( int j = 0; j < relativePoints[i].size(); j++ ) {
				double xRel = origin.first + x.first * (i - size / 2) * lengthOfSection +
					y.first * (j - size / 2) * lengthOfSection + z.first * points[i][j] * lengthOfSection;
				double yRel = origin.second + x.second * (i - size / 2) * lengthOfSection +
					y.second * (j - size / 2) * lengthOfSection + z.second * points[i][j] * lengthOfSection;
				relativePoints[i][j] = std::pair<double, double>( xRel, yRel );
			}
		} else {
			double xRel = origin.first + x.first * (i - size / 2) * lengthOfSection +
				 z.first * points[i][0] * lengthOfSection;
			double yRel = origin.second + x.second * (i - size / 2) * lengthOfSection +
				 z.second * points[i][0] * lengthOfSection;
			relativePoints[i][0] = std::pair<double, double>( xRel, yRel );
		}
	}
}