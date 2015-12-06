#include "PlotterDll.h"


namespace PlotterDll {

    PLOTTERDLL_API HWND drawGraph( HINSTANCE hInstance, int nCmdShow )
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

        if( !GraphWindow::RegisterClass( hInstance ) ) {
            //return 1;
        }

        // first  bool - is2D
        // second bool - isNeedToFillIf3D
        GraphWindow mainWindow( 700, 700, L"C:\\Users\\1\\Desktop\\Plotter\\Plotter\\PlotterDll\\ex.xml", false, false );

        if( !mainWindow.Create( hInstance, nCmdShow ) ) {
            //return NULL;
        }
        mainWindow.Show();

        return mainWindow.GetHandle();

        //MSG message;
        //BOOL getMessageResult = 0;
        //while( (getMessageResult = ::GetMessage( &message, NULL, 0, 0 )) != 0 ) {
        //    if( getMessageResult == -1 ) {
        //        return -1;
        //    }

        //    ::TranslateMessage( &message );
        //    ::DispatchMessage( &message );
        //}

        //Gdiplus::GdiplusShutdown( gdiplusToken );

        //return message.wParam;
    }

}