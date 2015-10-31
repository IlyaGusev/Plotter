#include <vector>
#include <Windowsx.h>

#include "graphWindow.h"
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

GraphWindow::GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D ) :
	windowWidth(width),
	windowHeight(height),
	graphInPoints( formulaPath, is2D, 80 )
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

	return handle;
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
		graphInPoints.turnRight();
		break;
	case VK_LEFT:
		graphInPoints.turnLeft();
		break;
	case VK_UP:
		graphInPoints.turnUp();
		break;
	case VK_DOWN:
		graphInPoints.turnDown();
		break;
	// Z key
	case 0x5A:
		graphInPoints.changeScale(-2 );
		break;
	// X key
	case 0x58:
		graphInPoints.changeScale( 2 );
		break;
	// D key
	case 0x44:
		graphInPoints.moveAlongX( 1 );
		break;
	// A key
	case 0x41:
		graphInPoints.moveAlongX( -1 );
		break;
	// W key
	case 0x57:
		graphInPoints.moveAlongY( 1 );
		break;
	// S key
	case 0x53:
		graphInPoints.moveAlongY( -1 );
		break;
	default:
		return;
	}
	::InvalidateRect( handle, NULL, FALSE );
	::UpdateWindow( handle );
}

void GraphWindow::OnMouseWheel( WPARAM wParam )
{
	if( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 ) {
		::SendMessage( handle, WM_KEYDOWN, 0x5A, 0 );
	} else {
		::SendMessage( handle, WM_KEYDOWN, 0x58, 0 );
	}
}

void GraphWindow::OnMouseMove( WPARAM wParam, int x, int y )
{
	if( wParam == MK_LBUTTON ) {
		if( x - prevMousePosX > 2 ) {
			::SendMessage( handle, WM_KEYDOWN, VK_RIGHT, 0 );
			prevMousePosX = x;
		} else if( x - prevMousePosX < -2 ) {
			::SendMessage( handle, WM_KEYDOWN, VK_LEFT, 0 );
			prevMousePosX = x;
		}
		if( y - prevMousePosY > 2 ) {
			::SendMessage( handle, WM_KEYDOWN, VK_DOWN, 0 );
			prevMousePosY = y;
		} else if( y - prevMousePosY < -2 ) {
			::SendMessage( handle, WM_KEYDOWN, VK_UP, 0 );
			prevMousePosY = y;
		}
	}
}

void GraphWindow::OnLButtonDown( int xMousePos, int yMousePos )
{
	prevMousePosX = xMousePos;
	prevMousePosY = yMousePos;
}

void GraphWindow::OnPaint()
{
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
	::SetBkColor(dc, RGB(0, 0, 0));

	HPEN linePen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	::SelectObject(dc, linePen);

	std::vector< std::vector < std::pair<double, double> > > points = graphInPoints.getRelativePoints();
	
	int pointsJSize = points[0].size() % 3 == 0 ? points[0].size() - 2 : 3 * (points[0].size() / 3) + 1;
	int pointsISize = points.size() % 3 == 0 ? points.size() - 2 : 3 * (points.size() / 3) + 1;

	for( size_t i = 0; i < pointsISize; ++i ) {
		POINT* lppoints = new POINT[pointsJSize];
		for( size_t j = 0; j < pointsJSize; ++j ) {
			lppoints[j] = { round( points[i][j].first ), round( points[i][j].second ) };
		}
		::PolyBezier( dc, lppoints, pointsJSize );

		delete[] lppoints;
	}

	for( size_t j = 0; j < pointsJSize; ++j ) {
		POINT* lppoints = new POINT[pointsISize];
		for( size_t i = 0; i < pointsISize; ++i ) {
			lppoints[i] = { round( points[i][j].first ), round( points[i][j].second ) };
		}
		::PolyBezier( dc, lppoints, pointsISize );

		delete[] lppoints;
	}

	::DeleteObject(linePen);
}

void GraphWindow::drawAxes(HDC dc) {
	std::pair<double, double> xAxis = graphInPoints.getAxisVectorVisual(0);
	std::pair<double, double> yAxis = graphInPoints.getAxisVectorVisual(1);
	std::pair<double, double> zAxis = graphInPoints.getAxisVectorVisual(2);

	std::pair<double, double> origin = graphInPoints.getOriginCoordinates();

	HPEN linePen = ::CreatePen(PS_SOLID, 1, RGB(100, 100, 200));
	::SelectObject(dc, linePen);
	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(xAxis.first * 10000000000) / 1000, round(xAxis.second * 10000000000) / 1000);
	::DeleteObject(linePen);

	std::string text = "X";
	::SetTextColor(dc, RGB(100, 100, 200));
	::TextOut(dc, round(origin.first + xAxis.first * 200), round(origin.second + xAxis.second * 200),
		(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length());

	linePen = ::CreatePen(PS_SOLID, 1, RGB(200, 100, 200));
	::SelectObject(dc, linePen);
	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(yAxis.first * 10000000000) / 1000, round(yAxis.second * 10000000000) / 1000);

	text = "Y";
	::SetTextColor(dc, RGB(200, 100, 200));
	::TextOut(dc, round(origin.first + yAxis.first * 200), round(origin.second + yAxis.second * 200),
		(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length());

	linePen = ::CreatePen(PS_SOLID, 1, RGB(100, 200, 200));
	::SelectObject(dc, linePen);
	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(zAxis.first * 10000000000) / 1000, round(zAxis.second * 10000000000) / 1000);
	::DeleteObject(linePen);

	text = "Z";
	::SetTextColor(dc, RGB(100, 200, 200));
	::TextOut(dc, round(origin.first + zAxis.first * 200), round(origin.second + zAxis.second * 200),
		(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length());
}

void GraphWindow::fillWithGradient(HDC dc) {
	Graphics graphics( dc );
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetSmoothingMode(SmoothingModeNone);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);
	graphics.SetTextRenderingHint(TextRenderingHintSingleBitPerPixel);
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

		case WM_MOUSEWHEEL:
			that->OnMouseWheel( wParam );
			return 0;

		case WM_LBUTTONDOWN:
			that->OnLButtonDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;

		case WM_MOUSEMOVE:
			that->OnMouseMove( wParam, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;

		case WM_ERASEBKGND:
			return 1;
	}
		
	return ::DefWindowProc(handle, message, wParam, lParam);
}