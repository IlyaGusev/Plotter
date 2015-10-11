#include <windows.h>
#include <vector>
#include "graphWindow.h"
#include "GP.h"

GraphWindow::GraphWindow(int width, int height, MathCore &mathCore) :
	windowWidth(width),
	windowHeight(height)
{
	GP graphInPoints = GP(/*mathCore*/);
}

GraphWindow::~GraphWindow()
{
}

const wchar_t* GraphWindow::nameClassWindow = L"ClassGraphWindow";
const wchar_t* GraphWindow::nameWindow = L"GraphWindow";

bool GraphWindow::RegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX tag;
	tag.cbSize = sizeof(WNDCLASSEX);
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	tag.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	tag.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	tag.lpszMenuName = NULL;
	tag.lpszClassName = nameClassWindow;
	tag.hInstance = hInstance;
	tag.hIconSm = NULL;

	if (!::RegisterClassEx(&tag)) {
		::MessageBox(NULL, L"Can't register class", L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

bool GraphWindow::Create(HINSTANCE hInstance, int nCmdShow) {
	cmdShow = nCmdShow;

	handle = ::CreateWindowEx( NULL, nameClassWindow, NULL, 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER,
		200, 20, windowWidth, windowHeight,
		NULL, NULL, hInstance, this);

	return true;
}

void GraphWindow::Show() {
	::ShowWindow(handle, cmdShow);
	::UpdateWindow(handle);
}

HWND GraphWindow::GetHandle() {
	return handle;
}

void GraphWindow::OnDestroy() {
	::PostQuitMessage(0);
}

void GraphWindow::OnClose() {
	int msgboxID = ::MessageBox(handle, (LPCWSTR)L"Do you surely want to exit?", (LPCWSTR)L"Exit",
		MB_YESNO | MB_ICONWARNING);

	switch (msgboxID) {
	case IDNO:
		return;
	}

	::DestroyWindow(handle);
}

void GraphWindow::OnKeyDown(WPARAM wParam) {
	switch (wParam) {
	case VK_RIGHT:
		graphInPoints.turnAroundY(1);
		::InvalidateRect(handle, NULL, FALSE);
		::UpdateWindow(handle);
		break;
	case VK_LEFT:
		graphInPoints.turnAroundY(-1);
		::InvalidateRect(handle, NULL, FALSE);
		::UpdateWindow(handle);
		break;
	case VK_UP:
		graphInPoints.turnAroundZ(1);
		::InvalidateRect(handle, NULL, FALSE);
		::UpdateWindow(handle);
		break;
	case VK_DOWN:
		graphInPoints.turnAroundZ(-1);
		::InvalidateRect(handle, NULL, FALSE);
		::UpdateWindow(handle);
		break;
	case 0x57:
		//graphInPoints.moveStraight();
		::InvalidateRect(handle, NULL, FALSE);
		::UpdateWindow(handle);
		break;
	case 0x53:
		//graphInPoints.moveBack();
		::InvalidateRect(handle, NULL, FALSE);
		::UpdateWindow(handle);
		break;
	}
}

void GraphWindow::OnPaint() {
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(handle, &ps);
	HDC newHdc = ::CreateCompatibleDC(hdc);
	RECT rect;
	::GetClientRect(handle, &rect);

	HBITMAP bitmap = ::CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
	HGDIOBJ oldbitmap = ::SelectObject(newHdc, bitmap);

	drawGraph(newHdc);
	drawAxes(newHdc);

	::BitBlt( hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY );

	::SelectObject(newHdc, oldbitmap);
	::DeleteObject(bitmap);

	::DeleteDC(hdc);
	::DeleteDC(newHdc);

	::ValidateRect(handle, NULL);
	::EndPaint(handle, &ps);
}

void GraphWindow::drawGraph(HDC dc) {
	RECT clientRect;
	::GetClientRect(handle, &clientRect);

	HBRUSH clientRectBrush = ::CreateSolidBrush(RGB(0, 0, 0));
	::FillRect(dc, &clientRect, clientRectBrush);
	::DeleteObject(clientRectBrush);

	HPEN linePen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	::SelectObject(dc, linePen);

	std:vector< std::vector < std::pair<double, double> > > points = graphInPoints.GetRelativePoints();

	for (size_t i = 0; i < points.size(); ++i) {
		::MoveToEx(dc, round(points[i][0].first), round(points[i][0].second), NULL);
		for (size_t j = 1; j < points.size(); ++j) {
			::LineTo(dc, round(points[i][j].first), round(points[i][j].second));
			::MoveToEx(dc, round(points[i][j].first), round(points[i][j].second), NULL);
		}
	}

	for (size_t j = 0; j < points.size(); ++j) {
		::MoveToEx(dc, round(points[0][j].first), round(points[0][j].second), NULL);
		for (size_t i = 1; i < points.size(); ++i) {
			::LineTo(dc, round(points[i][j].first), round(points[i][j].second));
			::MoveToEx(dc, round(points[i][j].first), round(points[i][j].second), NULL);
		}
	}

	::DeleteObject(linePen);
}

void GraphWindow::drawAxes(HDC dc) {
	pair<double, double> xAxis = graphInPoints.getAxisVectorVisual(0);
	pair<double, double> yAxis = graphInPoints.getAxisVectorVisual(1);
	pair<double, double> zAxis = graphInPoints.getAxisVectorVisual(2);

	pair<double, double> origin = graphInPoints.getOriginCoordinates();

	HPEN linePen = ::CreatePen(PS_SOLID, 1, RGB(100, 100, 220));
	::SelectObject(dc, linePen);

	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(xAxis.first * 10000000000) / 1000, round(xAxis.second * 10000000000) / 1000);

	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(yAxis.first * 10000000000) / 1000, round(yAxis.second * 10000000000) / 1000);

	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(zAxis.first * 10000000000) / 1000, round(zAxis.second * 10000000000) / 1000);

	::DeleteObject(linePen);
}

LRESULT __stdcall GraphWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_NCCREATE) {
		GraphWindow* that = reinterpret_cast< GraphWindow* >(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		::SetWindowLong(handle, GWL_USERDATA, reinterpret_cast<LONG>(that));

		return ::DefWindowProc(handle, message, wParam, lParam);
	}

	GraphWindow* that = reinterpret_cast< GraphWindow* >(::GetWindowLong(handle, GWL_USERDATA));

	switch (message) {
		case WM_CLOSE:
			that->OnClose();
			return 0;
		case WM_DESTROY:
			that->OnDestroy();
			return 0;
		case WM_PAINT:
			that->OnPaint();
			return 0;
		case WM_KEYDOWN:
			that->OnKeyDown(wParam);
			return 0;
		case WM_ERASEBKGND:
			return 1;
	}
		
	return ::DefWindowProc(handle, message, wParam, lParam);
}