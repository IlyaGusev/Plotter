#pragma once

#include <UIRibbon.h>
extern IUIFramework *g_pFramework;  // ССылка на Ribbon framework.
extern IUIApplication *g_pApplication;  // ССылка на объект приложения.
extern HWND g_pHwnd; //Хендл приложения

bool InitializeFramework(HWND hWnd);
void DestroyFramework();
