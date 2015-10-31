#include "RotationLib.h""
#include "math.h"



Vector::Vector(): x(1), y(1), z(1) {}

Vector::Vector( double inputX, double inputY, double inputZ ): x(inputX), y(inputY), z(inputZ) {}

void Vector::initialize( double inputX, double inputY, double inputZ ) {
	x = inputX;
	y = inputY;
	z = inputZ;
}
void Vector::normalize() {
	double norm = sqrt( x*x + y*y + z*z );
	x = x / norm;
	y = y / norm;
	z = z / norm;
}
void Vector::operator =( const Vector& vect ) {
	x = vect.x;
	y = vect.y;
	z = vect.z;
}


Quaternion::Quaternion(): w(0), x(0), y(0), z(1) {}

Quaternion::Quaternion( double inputAngle, const Vector& vect ) {
	double vnorm = sqrt( vect.x*vect.x + vect.y*vect.y + vect.z*vect.z );
	w = cos( inputAngle / 180 / 2 * M_PI );
	x = sin( inputAngle / 180 / 2 * M_PI ) * vect.x / vnorm;
	y = sin( inputAngle / 180 / 2 * M_PI ) * vect.y / vnorm;
	z = sin( inputAngle / 180 / 2 * M_PI ) * vect.z / vnorm;
}
Quaternion::Quaternion( double inputAngle, double inputX, double inputY, double inputZ ) {
	double vnorm = sqrt( inputX*inputX + inputY*inputY + inputZ*inputZ );
	w = cos( inputAngle / 180 / 2 * M_PI );
	x = sin( inputAngle / 180 / 2 * M_PI ) * inputX / vnorm;
	y = sin( inputAngle / 180 / 2 * M_PI ) * inputY / vnorm;
	z = sin( inputAngle / 180 / 2 * M_PI ) * inputZ / vnorm;
}
Vector Quaternion::makeRotation( const Vector& vect ) {
	// Создаем кватернион соответсвующий получаемому вектору
	Quaternion start;
	start.x = vect.x;
	start.y = vect.y;
	start.z = vect.z;
	start.w = 0;
	Quaternion inversed = this->getInversedQuaternion();
	Quaternion res = (*this) * start;
	res = res * inversed;
	double norm = sqrt( res.x*res.x + res.y*res.y + res.z*res.z );
	return Vector( res.x/norm, res.y/norm, res.z/norm );
}
void Quaternion::initialize( double inputAngle, const Vector& vect ) {
	double vnorm = sqrt( vect.x*vect.x + vect.y*vect.y + vect.z*vect.z );
	w = cos( inputAngle / 180 / 2 * M_PI );
	x = sin( inputAngle / 180 / 2 * M_PI ) * vect.x / vnorm;
	y = sin( inputAngle / 180 / 2 * M_PI ) * vect.y / vnorm;
	z = sin( inputAngle / 180 / 2 * M_PI ) * vect.z / vnorm;
}
void Quaternion::initialize( double inputAngle, double inputX, double inputY, double inputZ ) {
	double vnorm = sqrt( inputX*inputX + inputY*inputY + inputZ*inputZ );
	w = cos( inputAngle / 180 / 2 * M_PI );
	x = sin( inputAngle / 180 / 2 * M_PI ) * inputX / vnorm;
	y = sin( inputAngle / 180 / 2 * M_PI ) * inputY / vnorm;
	z = sin( inputAngle / 180 / 2 * M_PI ) * inputZ / vnorm;
}
void Quaternion::operator =( const Quaternion& quat ) {
	w = quat.w;
	x = quat.x;
	y = quat.y;
	z = quat.z;
}
Quaternion Quaternion::getInversedQuaternion() {
	return Quaternion( w, -x, -y, -z );
}
Quaternion Quaternion::operator *( const Quaternion& quat ) { 
	
	Quaternion q;
	q.w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
	q.x = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
	q.y = w * quat.y - x * quat.z + y * quat.w + z * quat.x;
	q.z = w * quat.z + x * quat.y - y * quat.x + z * quat.w;
	return q;
} 