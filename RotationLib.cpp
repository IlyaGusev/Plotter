#include "RotationLib.h""
#include "math.h"



Vector::Vector() {
	x = 1;
	y = 1;
	z = 1;
}
Vector::Vector( double inputX, double inputY, double inputZ ) {
	x = inputX;
	y = inputY;
	z = inputZ;
}
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



Quaternion::Quaternion() {
	w = 0;
	x = 0;
	y = 0;
	z = 1;
}
Quaternion::Quaternion( double inputAngle, const Vector& vect ) {
	double vnorm = sqrt( vect.x*vect.x + vect.y*vect.y + vect.z*vect.z );
	w = cos( inputAngle / 180 * M_PI );
	x = sin( inputAngle / 180 * M_PI ) * vect.x / vnorm;
	y = sin( inputAngle / 180 * M_PI ) * vect.y / vnorm;
	z = sin( inputAngle / 180 * M_PI ) * vect.z / vnorm;
}
Quaternion::Quaternion( double inputAngle, double inputX, double inputY, double inputZ ) {
	double vnorm = sqrt( inputX*inputX + inputY*inputY + inputZ*inputZ );
	w = cos( inputAngle / 180 * M_PI );
	x = sin( inputAngle / 180 * M_PI ) * inputX / vnorm;
	y = sin( inputAngle / 180 * M_PI ) * inputY / vnorm;
	z = sin( inputAngle / 180 * M_PI ) * inputZ / vnorm;
}
Vector Quaternion::makeRotation( const Vector& vect ) {
	// Создаем кватернион соответсвующий получаемому вектору
	Quaternion start( 0, vect );
	Quaternion inversed = this->getInversedQuaternion();
	Quaternion res = start * inversed;
	res = (*this) * res;
	return Vector( res.x, res.y, res.z );
}
void Quaternion::initialize( double inputAngle, const Vector& vect ) {
	double vnorm = sqrt( vect.x*vect.x + vect.y*vect.y + vect.z*vect.z );
	w = cos( inputAngle / 180 * M_PI );
	x = sin( inputAngle / 180 * M_PI ) * vect.x / vnorm;
	y = sin( inputAngle / 180 * M_PI ) * vect.y / vnorm;
	z = sin( inputAngle / 180 * M_PI ) * vect.z / vnorm;
}
void Quaternion::initialize( double inputAngle, double inputX, double inputY, double inputZ ) {
	double vnorm = sqrt( inputX*inputX + inputY*inputY + inputZ*inputZ );
	w = cos( inputAngle / 180 * M_PI );
	x = sin( inputAngle / 180 * M_PI ) * inputX / vnorm;
	y = sin( inputAngle / 180 * M_PI ) * inputY / vnorm;
	z = sin( inputAngle / 180 * M_PI ) * inputZ / vnorm;
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
	double resW = w*quat.w - x*quat.x - y*quat.y - z*quat.z;
	double resX = x*quat.w + w*quat.x - z*quat.y + y*quat.z;
	double resY = y*quat.w + y*quat.x + w*quat.y - x*quat.z;
	double resZ = z*quat.w - y*quat.x + x*quat.y + w*quat.z;
	return Quaternion( resW, resX, resY, resZ );
} 