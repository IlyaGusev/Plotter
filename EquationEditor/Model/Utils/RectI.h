#pragma once

class CRectI {
public:
	CRectI();
	CRectI( int _left, int _top, int _right, int _bottom );
	~CRectI();

	void MoveBy( int dx, int dy );

	void MoveTo( int newLeft, int newTop );

	void Set( int left, int top, int right, int bottom );

	bool isContain( int x, int y ) const;

	CRectI& CRectI::operator=(const CRectI& rect);

	bool operator==( CRectI& rect ) const;

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

