#include "RectI.h"

CRectI::CRectI() :
	left( 0 ), right( 0 ), top( 0 ), bottom( 0 ) {}

CRectI::CRectI( int _left, int _top, int _right, int _bottom ) :
	left( _left ),
	top( _top ),
	right( _right ),
	bottom( _bottom )
{
}

CRectI::~CRectI() {}

void CRectI::MoveBy( int dx, int dy ) {
	left += dx;
	right += dx;
	top += dy;
	bottom += dy;
}

void CRectI::MoveTo( int newLeft, int newTop ) {
	MoveBy( newLeft - left, newTop - top );
}

void CRectI::Set( int left, int top, int right, int bottom ) {
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}

bool CRectI::isContain( int x, int y ) const {
	return top <= y && y <= bottom && left <= x && x <= right;
}

int& CRectI::Left() {
	return left;
}

int& CRectI::Right() {
	return right;
}

int& CRectI::Top() {
	return top;
}

int& CRectI::Bottom() {
	return bottom;
}

CRectI& CRectI::operator=(const CRectI& rect) {
	if( this == &rect ) {
		return *this;
	}
	this->left = rect.left;
	this->right = rect.right;
	this->top = rect.top;
	this->bottom = rect.bottom;
	return *this;
}

bool CRectI::operator==(CRectI& rect) const {
	return left == rect.left && right == rect.right && 
		bottom == rect.bottom && top == rect.top;
}
