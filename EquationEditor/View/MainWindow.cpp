#include "MainWindow.h"
#include <Windowsx.h>
#include "resource.h"
#include "../RibbonFramework.h"

CMainWindow::CMainWindow() : hwnd(nullptr)
{
}

bool CMainWindow::RegisterClassW()
{
  WNDCLASSEX wnd;
  ZeroMemory(&wnd, sizeof(wnd));
  wnd.cbSize = sizeof(wnd);
  wnd.style = CS_HREDRAW | CS_VREDRAW;
  wnd.lpfnWndProc = mainWindowProc;
  wnd.hInstance = ::GetModuleHandle(nullptr);
  wnd.hIcon = reinterpret_cast<HICON>(::LoadImage(wnd.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
  wnd.hIconSm = reinterpret_cast<HICON>(::LoadImage(wnd.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
  wnd.lpszClassName = L"MainWindow";
  wnd.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wnd.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
  
  CEquationEditorWindow::RegisterClassW();
  return ::RegisterClassEx(&wnd) != 0;
}

UINT32 CMainWindow::getRibbonHeight()
{
  UINT32 ribbonHeight;
  HRESULT hr = E_FAIL;
  IUIRibbon* pRibbon;
  hr = g_pFramework->GetView(0, IID_PPV_ARGS(&pRibbon));
  if (SUCCEEDED(hr)) {
    UINT32 uRibbonHeight = 0;
    hr = pRibbon->GetHeight(&uRibbonHeight);
    if (SUCCEEDED(hr)) {
      ribbonHeight = uRibbonHeight;
    }
    pRibbon->Release();
  }
  if (FAILED(hr)) {
    ribbonHeight = 0;
  }

  return ribbonHeight;
}

RECT CMainWindow::getChildRect() {
  RECT rect;
  ::GetClientRect(GetHandle(), &rect);
  RECT childRect;
  childRect.left = 0;
  childRect.top = getRibbonHeight();
  childRect.bottom = rect.bottom - getRibbonHeight();
  childRect.right = rect.right;

  return childRect;
}

bool CMainWindow::Create()
{
  if ( ::CreateWindowEx( 0, L"MainWindow", L"Equation Editor", WS_OVERLAPPEDWINDOW | WS_EX_LAYERED | WS_CLIPCHILDREN, 0, 0, 800, 600,
    nullptr, nullptr, ::GetModuleHandle(nullptr), this ) != 0 ) {
    
    return ( editor.Create( GetHandle(), getChildRect() ) != 0 );
  }
  return false;
}

void CMainWindow::Show(int cmdShow)
{
  ::ShowWindow(hwnd, cmdShow);
  editor.Show(cmdShow);
}

void CMainWindow::OnDestroy()
{
  ::PostQuitMessage(0);
}

void CMainWindow::OnCreate()
{
}

void CMainWindow::OnSize(int cxSize, int cySize)
{
  RECT child = getChildRect();
  ::MoveWindow( editor.GetHandle(), child.left, child.top, child.right - child.left, child.bottom, TRUE );
}

void CMainWindow::Redraw()
{
  RECT rctB = { 0, 0, 1800, 1800 };
  ::InvalidateRect(hwnd, &rctB, TRUE);
}


void CMainWindow::OnWmCommand(WPARAM wParam, LPARAM lParam)
{
  editor.OnWmCommand(wParam, lParam);
}

void CMainWindow::OnKeyDown(WPARAM wParam)
{
  editor.OnKeyDown(wParam);
}

void CMainWindow::OnMouseMove(WPARAM wParam, int x, int y)
{
  editor.OnMouseMove(wParam, x, y);
}

void CMainWindow::OnChar(WPARAM wParam)
{
  editor.OnChar(wParam);
}

void CMainWindow::OnDraw()
{
  PAINTSTRUCT ps;
  HDC curhdc = ::BeginPaint(hwnd, &ps);

  RECT rect;
  ::GetClientRect(hwnd, &rect);
  HDC hdc = ::CreateCompatibleDC(curhdc);
  HBITMAP backbuffer = ::CreateCompatibleBitmap(curhdc, rect.right - rect.left, rect.bottom - rect.top);
  ::SelectObject(hdc, backbuffer);
  // ::FillRect(hdc, &rect, ::CreateSolidBrush(bkUnselectedColorref));

  editor.OnDraw();

  ::BitBlt(curhdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hdc, 0, 0, SRCCOPY);

  ::DeleteObject(backbuffer);
  ::DeleteDC(hdc);

  ::EndPaint(hwnd, &ps);
}


LRESULT CMainWindow::mainWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
  CMainWindow *wnd = nullptr;

  if (message == WM_NCCREATE) {
    // �������� ��������� �� ��������� ������ ����, ������� �� �������� � ������� CreateWindowEx
    wnd = static_cast<CMainWindow*>(LPCREATESTRUCT(lParam)->lpCreateParams);
    // � ��������� � ���� GWL_USERDATA
    ::SetWindowLong(handle, GWL_USERDATA, reinterpret_cast<LONG>(LPCREATESTRUCT(lParam)->lpCreateParams));
    // ���������� handle
    wnd->hwnd = handle;
  }
  // ������ �������� ��������� �� ��� ��������� ����, �� ��� �� ���� GWL_USERDATA
  wnd = reinterpret_cast<CMainWindow*>(::GetWindowLong(handle, GWL_USERDATA));

  switch (message) {
  case WM_DESTROY:
    DestroyFramework();
    wnd->OnDestroy();
    return 0;

  case WM_CREATE:
    InitializeFramework(handle);
    wnd->OnCreate();
    return 0;

  case WM_PAINT:
    wnd->OnDraw();
    return 0;

  case WM_SIZE:
    wnd->OnSize(LOWORD(lParam), HIWORD(lParam));
    return 0;

  case WM_CHAR:
    wnd->OnChar(wParam);
    return 0;

  /*case WM_LBUTTONDOWN:
    wnd->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;*/

  case WM_MOUSEMOVE:
    wnd->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    return 0;

  case WM_COMMAND:
    wnd->OnWmCommand(wParam, lParam);
    return 0;

  case WM_KEYDOWN:
    wnd->OnKeyDown(wParam);
    return 0;

  case WM_ERASEBKGND:
    return 0;
  }
  return ::DefWindowProc(handle, message, wParam, lParam);
}
