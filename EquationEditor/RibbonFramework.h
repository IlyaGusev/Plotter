#pragma once

#include <UIRibbon.h>
extern IUIFramework *g_pFramework;  // ������ �� Ribbon framework.
extern IUIApplication *g_pApplication;  // ������ �� ������ ����������.
extern HWND g_pHwnd; //����� ����������

bool InitializeFramework(HWND hWnd);
void DestroyFramework();
