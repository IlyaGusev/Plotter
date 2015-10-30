#include <Windows.h>
#include "RibbonFramework.h"
#include "View/EquationEditorWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	FILE *stream;
	freopen_s(&stream, "output.txt", "w", stdout);
	HRESULT hr = CoInitialize(NULL);
    CEquationEditorWindow::RegisterClass();
    CEquationEditorWindow wnd;
    wnd.Create();
    wnd.Show(nCmdShow);

    MSG msg;
    while (::GetMessage(&msg, (HWND)nullptr, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

	fclose(stream);
    return 0;
}