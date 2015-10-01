#pragma once
#include <windows.h>
#include "GP.h"

class GraphWindow {
public:
	GraphWindow(int width, int height);
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
	static wchar_t* nameClassWindow;
	static wchar_t* nameWindow;

	int windowWidth;
	int windowHeight;

	int cmdShow;
	HWND handle;
	
	GP graphInPoints;

	void drawGraph(HDC dc);
	void drawAxes(HDC dc);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};

