#pragma once
#include <Windows.h>

#include "mathCore.h"
#include "GP.h"

class GraphWindow {
public:
	GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D = false );
	//~GraphWindow();

	static bool RegisterClass(HINSTANCE hInstance);

	bool Create(HINSTANCE hInstance, int nCmdShow);

	void Show();

	HWND GetHandle();

protected:
	void OnKeyDown( WPARAM wParam );
	void OnMouseWheel( WPARAM wParam );
	void OnMouseMove( WPARAM wParam, int x, int y );
	void OnLButtonDown( int xMousePos, int yMousePos );

	void OnPaint();
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
	
	GP graphInPoints;

	void drawGraph(HDC dc);
	void drawAxes(HDC dc);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

