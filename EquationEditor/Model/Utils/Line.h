#pragma once

class CLine {
public:
	CLine();

	CLine( int _startX, int _startY, int _endX, int _endY );

	void Set( int startX, int startY, int endX, int endY );
	
	void MoveBy( int dx, int dy );

	int& StartX();
	int& StartY();
	int& EndX();
	int& EndY();
private:
	int startX;
	int startY;
	int endX;
	int endY;
};

