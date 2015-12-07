#include "PlotterDll.h"


namespace PlotterDll {

    bool isRegistered = false;


    PLOTTERDLL_API HWND drawGraph( int nCmdShow, const char* path )
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

        if( !mainWindow.Create() ) {
            //return NULL;
        }
        mainWindow.Show( nCmdShow );

//        return mainWindow.GetHandle();

        MSG message;
        BOOL getMessageResult = 0;
        while( (getMessageResult = ::GetMessage( &message, NULL, 0, 0 )) != 0 ) {
            if( getMessageResult == -1 ) {
                //return -1;
            }

            ::TranslateMessage( &message );
            ::DispatchMessage( &message );
        }

        Gdiplus::GdiplusShutdown( gdiplusToken );

        return mainWindow.GetHandle();
        //return message.wParam;
    }

}