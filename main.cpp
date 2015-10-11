#include <windows.h>
#include <string>
#include "graphWindow.h"
#include "mathCore.h"

int _stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR commandLine, int nCmdShow) {
	if (!GraphWindow::RegisterClass(hInstance)) {
		return 1;
	}

	std::string formula = "(x + y + 1) * 4";
	MathCore mathCore(formula);

	GraphWindow* mainWindow = new GraphWindow(700, 700, mathCore);

	if (!mainWindow->Create(hInstance, nCmdShow)) {
		return 1;
	}

	mainWindow->Show();

	MSG message;
	BOOL getMessageResult = 0;
	while ((getMessageResult = ::GetMessage(&message, NULL, 0, 0)) != 0) {
		if (getMessageResult == -1) {
			return -1;
		}

		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}

	delete(mainWindow);

	return message.wParam;
}