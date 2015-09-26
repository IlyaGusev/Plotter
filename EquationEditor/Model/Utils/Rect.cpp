#include "Rect.h"

CRect::CRect() :
	left( 0 ), right( 0 ), top( 0 ), bottom( 0 ) {}

CRect::CRect( int _left, int _top, int _right, int _bottom ) :
	left( _left ),
	top( _top ),
	right( _right ),
	bottom( _bottom )
{
}

void CRect::MoveBy( int dx, int dy ) {
	left += dx;
	right += dx;
	top += dy;
	bottom += dy;
}

void CRect::MoveTo( int newLeft, int newTop ) {
	MoveBy( newLeft - left, newTop - top );
}

void CRect::Set( int left, int top, int right, int bottom ) {
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}

bool CRect::isContain( int x, int y ) const {
	return top <= y && y <= bottom && left <= x && x <= right;
}

int& CRect::Left() {
	return left;
}

int& CRect::Right() {
	return right;
}

int& CRect::Top() {
	return top;
}

int& CRect::Bottom() {
	return bottom;
}

CRect& CRect::operator=(const CRect& rect) {
	if( this == &rect ) {
		return *this;
	}
	this->left = rect.left;
	this->right = rect.right;
	this->top = rect.top;
	this->bottom = rect.bottom;
	return *this;
}

bool CRect::operator==( const CRect& rect ) const {
	return left == rect.left && right == rect.right && 
		bottom == rect.bottom && top == rect.top;
}
