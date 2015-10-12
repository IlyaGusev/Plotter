#pragma once
#include <windows.h>
#include "mathCore.h"
#include "GP.h"

class GraphWindow {
public:
	GraphWindow(int width, int height, MathCore& mathCore);
	~GraphWindow();

	static bool RegisterClass(HINSTANCE hInstance);

	bool Create(HINSTANCE hInstance, int nCmdShow);

	void Show();

	HWND GetHandle();

protected:
	void OnKeyDown(WPARAM wParam);

	void OnPaint();
	void OnClose();
	void OnDestroy();

private:
	const static wchar_t* nameClassWindow;
	const static wchar_t* nameWindow;

	int windowWidth;
	int windowHeight;

	int cmdShow;
	HWND handle;
	
	GP graphInPoints;

	void drawGraph(HDC dc);
	void drawAxes(HDC dc);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

