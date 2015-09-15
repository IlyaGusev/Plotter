#include <Windows.h>
#include "EquationEditorWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    CEquationEditorWindow::RegisterClass();
    CEquationEditorWindow wnd;
    wnd.Create();
    wnd.Show(nCmdShow);

    MSG msg;
    while (::GetMessage(&msg, (HWND)NULL, 0, 0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    return 0;
}