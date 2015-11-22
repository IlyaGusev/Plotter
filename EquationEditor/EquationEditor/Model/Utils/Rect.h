#pragma once

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

class CRect {
public:
	CRect();
	CRect( int _left, int _top, int _right, int _bottom );

	void MoveBy( int dx, int dy );

	void MoveTo( int newLeft, int newTop );

	void Set( int left, int top, int right, int bottom );

	// ������� ������ ����� �� dx ������, � ������ �� dy ����
	void Resize( int dx, int dy );

	// �������� �� �����
	bool IsContain( int x, int y ) const;

	// ������������ �� ��� ��������������
	bool isIntersect( const CRect& other ) const;

	CRect& CRect::operator= (const CRect& rect );

	bool operator==( const CRect& rect ) const;

	int& Left();
	int& Right();
	int& Top();
	int& Bottom( );
	const int Left( ) const;
	const int Right( ) const;
	const int Top( ) const;
	const int Bottom( ) const;

	const int GetWidth( ) const;
	const int GetHeight( ) const;

private:
	int left;
	int top;
	int right;
	int bottom;
};

