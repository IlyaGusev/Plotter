#pragma once
#include <Windows.h>
#include <gdiplus.h>

#include "GP.h"

struct Polygon4Wrap {
	Gdiplus::PointF poly[4];
};

class GraphWindow {
public:
	GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D = false, bool isFillPolygonsIf3D = true );

	static bool RegisterClass(HINSTANCE hInstance);

	bool Create(HINSTANCE hInstance, int nCmdShow);

	void Show();

	HWND GetHandle();

protected:
	void OnKeyDown( WPARAM wParam );
	void OnMouseWheel( WPARAM wParam );
	void OnMouseMove( WPARAM wParam, int x, int y );
	void OnLButtonDown( int xMousePos, int yMousePos );
	void OnImageSave();

	void OnPaint();
	void OnSize(int width, int height);
	void OnClose();
	void OnDestroy();

private:
	const static wchar_t* nameClassWindow;
	const static wchar_t* nameWindow;

	int prevMousePosX;
	int prevMousePosY;

	int windowWidth;
	int windowHeight;

	int cmdShow;
	HWND handle;
	HMENU menu;

	GP graphInPoints;

	bool needToFillPolygons;

	void drawGraph(HDC dc);
	void drawAxe(HDC dc, int axisNum, RECT rec, const std::string axisName);
	void drawAxes(HDC dc);
	void drawCoordinates(HDC dc, int axisNum, double maxValue, int pointsCount = 6);

	// red for max and blue for min by default
	void fillWithGradient(HDC dc, Gdiplus::Color maxColor = Gdiplus::Color(128, 255, 0, 0), Gdiplus::Color minColor = Gdiplus::Color(128, 0, 0, 255));

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	void getMaxMinZAndRelativeGridKnots( double& min, double& max, int& xMin, int& yMin, int& xMax, int& yMax );
	void getAllPolygonsOfGrid(std::vector< Polygon4Wrap > &polygons, std::vector< std::vector < std::pair<double, double> > > &points);
	void generatePointsOfMaxAndMinGradientColor( Gdiplus::Point &maxColorPoint, Gdiplus::Point &minColorPoint, 
												double& min, double& max, int& xMin, int& yMin, int& xMax, int& yMax, 
												std::vector< std::vector < std::pair<double, double> > > &points);
	bool ScreenCapture( LPWSTR filename );
};

