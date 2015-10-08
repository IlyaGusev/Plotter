#include "Line.h"

CLine::CLine() {}

CLine::CLine( int _startX, int _startY, int _endX, int _endY ) :
	startX( _startX ),
	startY( _startY ),
	endX( _endX ),
	endY( _endY ) {}

void CLine::Set( int startX, int startY, int endX, int endY ) {
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;
}

void CLine::MoveBy( int dx, int dy ) {
	startX += dx;
	startY += dy;
	endX += dx;
	endY += dy;
}

int& CLine::StartX() {
	return startX;
}

int& CLine::StartY() {
	return startY;
}

int& CLine::EndX() {
	return endX;
}

int& CLine::EndY() {
	return endY;
}
