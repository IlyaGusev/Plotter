#pragma once

class CRect {
public:
	CRect();
	CRect( int _left, int _top, int _right, int _bottom );

	void MoveBy( int dx, int dy );

	void MoveTo( int newLeft, int newTop );

	void Set( int left, int top, int right, int bottom );

	bool isContain( int x, int y ) const;

	CRect& CRect::operator= (const CRect& rect );

	bool operator==( const CRect& rect ) const;

	int& Left();
	int& Right();
	int& Top();
	int& Bottom();
private:
	int left;
	int top;
	int right;
	int bottom;
};

