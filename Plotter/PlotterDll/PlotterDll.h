// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PLOTTERDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PLOTTERDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PLOTTERDLL_EXPORTS
#define PLOTTERDLL_API __declspec(dllexport)
#else
#define PLOTTERDLL_API __declspec(dllimport)
#endif


#include <string>
#include <stdio.h>
#include <windows.h>
#include "graphWindow.h"


namespace PlotterDll {
    PLOTTERDLL_API HWND showGraph( HINSTANCE hInstance, int nCmdShow );
}
