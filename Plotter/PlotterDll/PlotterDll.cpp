#include "PlotterDll.h"


namespace PlotterDll {

    bool isRegistered = false;


    PLOTTERDLL_API int drawGraph( int nCmdShow, const char* path )
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

        if( !isRegistered ) {
            GraphWindow::RegisterClass();
            isRegistered = true;
        }

        // first  bool - is2D
        // second bool - isNeedToFillIf3D
        GraphWindow mainWindow( 700, 700, path, false, false );
        mainWindow.Create();
        mainWindow.Show( nCmdShow );

        MSG message;
        while( ::GetMessage( &message, NULL, 0, 0 ) ) {
            ::TranslateMessage( &message );
            ::DispatchMessage( &message );
        }

        Gdiplus::GdiplusShutdown( gdiplusToken );
        return message.wParam;
    }

}