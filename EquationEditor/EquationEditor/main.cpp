#include <Windows.h>
#include "RibbonFramework.h"
#include "View\MainWindow.h"
#include "resource.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	FILE *stream;
	freopen_s(&stream, "output.txt", "w", stdout);

    HRESULT hr = CoInitialize(NULL);
    CMainWindow::RegisterClassW();
    CMainWindow wnd;
    wnd.Create();
    wnd.Show(nCmdShow);

    //auto graphHandle = PlotterDll::drawGraph( hInstance, nCmdShow );

	HACCEL table = ::LoadAccelerators( hInstance, MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );
    MSG msg;
    while (::GetMessage(&msg, (HWND)nullptr, 0, 0) ) {
		if ( !::TranslateAccelerator( wnd.GetHandle(), table, &msg ) ) {
		    ::TranslateMessage( &msg );
		    ::DispatchMessage( &msg );
		}
        //if( !::TranslateAccelerator( graphHandle, table, &msg ) ) {
        //    ::TranslateMessage( &msg );
        //    ::DispatchMessage( &msg );
        //}
    }

	fclose(stream);
    return 0;
}