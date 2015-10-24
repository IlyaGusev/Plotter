#include <windows.h>
#include <string>
#include "graphWindow.h"
#include "mathCore.h"
#include <functional>
#include <list>
#include "./pugixml/pugixml.hpp"

int _stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR commandLine, int nCmdShow) {
	if (!GraphWindow::RegisterClass(hInstance)) {
		return 1;
	}

	std::string formula = "x * x + y * y";
	char* pathToFile = "./ex.xml";
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file( pathToFile );
	//MathCore mathCore( formula );
	MathCore mathCore(doc);

	//GraphWindow* mainWindow = new GraphWindow(700, 700, mathCore);
	GraphWindow mainWindow( 700, 700, mathCore );

	if(/*!mainWindow->Create(hInstance, nCmdShow)*/ !mainWindow.Create( hInstance, nCmdShow ) ) {
		return 1;
	}
	
	std::function<int(std::list<int>)> a = [](std::list<int> x){ int res = 0;
	std::function<int(int, int)> b = [](int a, int b){return a + b; };
	for (auto i = x.begin(); i != x.end(); ++i) {
		res += *i + b(4, 2);
	}
	return res;
	};
	//mainWindow->Show();
	mainWindow.Show();

	MSG message;
	BOOL getMessageResult = 0;
	while ((getMessageResult = ::GetMessage(&message, NULL, 0, 0)) != 0) {
		if (getMessageResult == -1) {
			return -1;
		}

		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}

	//delete(mainWindow);

	return message.wParam;
}