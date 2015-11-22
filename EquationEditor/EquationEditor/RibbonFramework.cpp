#include "RibbonFramework.h"
#include "Application.h"
#include "Presenter\EquationPresenter.h"

IUIFramework *g_pFramework = NULL;  // ������ �� Ribbon framework.
IUIApplication *g_pApplication = NULL;  // ������ �� ������ ����������.
HWND g_pHwnd; //����� �������� (��������� ����������)

//
//  FUNCTION: InitializeFramework(HWND)
//
//  PURPOSE:  ������������� � �������� � ����������
//
//
bool InitializeFramework(HWND hWnd)
{
	// �������� �������
	HRESULT hr = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pFramework));
	if (FAILED(hr))
	{
		return false;
	}

	// ������� ������ ����������, 
	// ��������� Ribbon � Application.
	hr = CApplication::CreateInstance(&g_pApplication);
	if (FAILED(hr))
	{
		return false;
	}

	hr = g_pFramework->Initialize(hWnd, g_pApplication);
	if (FAILED(hr))
	{
		return false;
	}
	g_pHwnd = hWnd;

	// ���������� ����, ����������� �������� 
	hr = g_pFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void DestroyFramework()
{
	if (g_pFramework)
	{
		g_pFramework->Destroy();
		g_pFramework->Release();
		g_pFramework = NULL;
	}

	if (g_pApplication)
	{
		g_pApplication->Release();
		g_pApplication = NULL;
	}
}
