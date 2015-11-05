#include <vector>
#include <Windowsx.h>

#include "graphWindow.h"
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

GraphWindow::GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D /*= false*/, bool isFillPolygonsIf3D /*= true */ ) :
	windowWidth(width),
	windowHeight(height),
	graphInPoints( formulaPath, is2D, 80 ),
	needToFillPolygons( isFillPolygonsIf3D )
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

	HPEN linePen;
	if (needToFillPolygons) {
		linePen = ::CreatePen( PS_SOLID, 1, RGB( 0, 100, 0 ) );
	} else {
		linePen = ::CreatePen( PS_SOLID, 1, RGB( 0, 255, 0 ) );
	}
	::SelectObject(dc, linePen);

	std::vector< std::vector < std::vector< std::pair<double, double> > > > points = graphInPoints.getRelativePoints();
	std::vector< std::vector < std::vector< PointF > > > yPolygonPoints;
	std::vector< std::vector < std::vector< PointF > > > zPolygonPoints;

	int iSize = points.size();
	int jSize = points[0].size();
	// Проводим отрезки вдоль оси Y
	for( size_t i = 0; i < iSize; ++i ) {
		yPolygonPoints.push_back( std::vector < std::vector< PointF > >() );
		zPolygonPoints.push_back( std::vector < std::vector< PointF > >() );
		zPolygonPoints.back().push_back( std::vector<PointF>() );

		size_t j = 0;
		// Ищем, где точки начинаются
		for( ; j < jSize && points[i][j].size( ) == 0; ++j );
		// Считаем, что есть k слоев, рисуем отрезки послойно
		for( size_t k = 0; k < ((j < jSize) ? points[i][j].size( ) : 0); ++k ) {
			yPolygonPoints.back().push_back( std::vector<PointF>() );

			POINT* lppoints = new POINT[jSize - j];
			size_t size = 0;
			size_t lastPoint = 0;
			// Проводим отрезок, соединяющий этот и следующий слой (вообще говоря, это нужно не всегда...)
			if( k + 1 < points[i][j].size() ) {
				::MoveToEx( dc, round( points[i][j][k].first ), round( points[i][j][k].second ), NULL );
				::LineTo( dc, round( points[i][j][k + 1].first ), round( points[i][j][k + 1].second ) );
				zPolygonPoints.back().back().push_back( PointF( points[i][j][k].first, points[i][j][k].second ) );
				zPolygonPoints.back().back().push_back( PointF( points[i][j][k + 1].first, points[i][j][k + 1].second ) );
			}
			for( size_t l = j; l < jSize; ++l ) {
				// Если в этой точке достаточно слоев - проводим отрезок через нее
				if( points[i][l].size() > k ) {
					// Если размер равен нулю и l != j - значит, среди отрезков на этой прямой был разрыв, заполняем его
					if( size == 0 && l != j ) {
						::LineTo( dc, round( points[i][l][k].first ), round( points[i][l][k].second ) );
					}
					lppoints[size] = { round( points[i][l][k].first ), round( points[i][l][k].second ) };
				
					yPolygonPoints.back().back().push_back( PointF( points[i][l][k].first, points[i][l][k].second ) );
					++size;
					lastPoint = l;
				} else {
					// Если слоев недостаточно, начинаем проводить кривые Безье по предыдущим точкам
					// Точек должно быть 3k+1
					int bezierPointsSize = size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1;
					::PolyBezier( dc, lppoints, bezierPointsSize );
					// Оставшиеся точки соединяем линией
					for( int p = bezierPointsSize - size; p < 0; ++p ) {
						if( k < points[i][l + p - 1].size() && k < points[i][l + p].size() ) {
							::MoveToEx( dc, round( points[i][l + p - 1][k].first ), round( points[i][l + p - 1][k].second ), NULL );
							::LineTo( dc, round( points[i][l + p][k].first ), round( points[i][l + p][k].second ) );
							yPolygonPoints.back().back().push_back( PointF( points[i][l + p - 1][k].first, points[i][l + p - 1][k].second ) );
							yPolygonPoints.back().back().push_back( PointF( points[i][l + p][k].first, points[i][l + p][k].second ) );
						}
					}
					size = 0;
					if( k < points[i][l - 1].size() ) {
						::MoveToEx( dc, round( points[i][l - 1][k].first ), round( points[i][l - 1][k].second ), NULL );
						yPolygonPoints.back().back().push_back( PointF( points[i][l - 1][k].first, points[i][l - 1][k].second ) );
					}
				}
			}

			::PolyBezier( dc, lppoints, size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1 );
			delete[] lppoints;

			// Проводим отрезок в верхний слой
			if( k + 1 < points[i][lastPoint].size() ) {
				::MoveToEx( dc, round( points[i][lastPoint][k].first ), round( points[i][lastPoint][k].second ), NULL );
				zPolygonPoints.back().back().push_back( PointF( points[i][lastPoint][k].first, points[i][lastPoint][k].second ) );
				::LineTo( dc, round( points[i][lastPoint][k + 1].first ), round( points[i][lastPoint][k + 1].second ) );
				zPolygonPoints.back().back().push_back( PointF( points[i][lastPoint][k + 1].first, points[i][lastPoint][k + 1].second ) );
				::MoveToEx( dc, round( points[i][lastPoint][k].first ), round( points[i][lastPoint][k].second ), NULL );
			}
		}
	}

	// Проводим отрезки вдоль оси X
	for( size_t j = 0; j < jSize; ++j ) {
		size_t i = 0;
		// Ищем, где точки начинаются
		for( ; i < iSize && points[i][j].size() == 0; ++i );
		// Считаем, что есть k слоев, рисуем отрезки послойно
		for( size_t k = 0; k < ((i < iSize) ? points[i][j].size() : 0); ++k ) {
			POINT* lppoints = new POINT[iSize - i];
			size_t size = 0;
			size_t lastPoint = 0;
			// Проводим отрезок, соединяющий этот и следующий слой (вообще говоря, это нужно не всегда...)
			if( k + 1 < points[i][j].size() ) {
				::MoveToEx( dc, round( points[i][j][k].first ), round( points[i][j][k].second ), NULL );
				::LineTo( dc, round( points[i][j][k + 1].first ), round( points[i][j][k + 1].second ) );
			}

			for( size_t l = i; l < iSize; ++l ) {
				// Если в этой точке достаточно слоев - проводим отрезок через нее
				if( points[l][j].size() > k ) {
					// Если размер равен нулю и l != i - значит, среди отрезков на этой прямой был разрыв, заполняем его
					if( size == 0 && l != i ) {
						::LineTo( dc, round( points[l][j][k].first ), round( points[l][j][k].second ) );
					}
					lppoints[size] = { round( points[l][j][k].first ), round( points[l][j][k].second ) };
					++size;
					lastPoint = l;
				} else {
					// Если слоев недостаточно, начинаем проводить кривые Безье
					// Точек должно быть 3k+1
					int bezierPointsSize = size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1;
					::PolyBezier( dc, lppoints, bezierPointsSize );
					// Оставшиеся точки соединяем линией
					for( int p = bezierPointsSize - size; p < 0; ++p ) {
						if( k < points[l + p - 1][j].size( ) && k < points[l + p][j].size( ) ) {
							::MoveToEx( dc, round( points[l + p - 1][j][k].first ), round( points[l + p - 1][j][k].second ), NULL );
							::LineTo( dc, round( points[l + p][j][k].first ), round( points[l + p][j][k].second ) );
						}
					}
					size = 0;
					if( k < points[l - 1][j].size() ) {
						::MoveToEx( dc, round( points[l - 1][j][k].first ), round( points[l - 1][j][k].second ), NULL );
					}
				}
			}

			::PolyBezier( dc, lppoints, size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1 );
			delete[] lppoints;

			// Проводим отрезок в верхний слой
			if( k + 1 < points[lastPoint][j].size( ) ) {
				::MoveToEx( dc, round( points[lastPoint][j][k].first ), round( points[lastPoint][j][k].second ), NULL );
				::LineTo( dc, round( points[lastPoint][j][k + 1].first ), round( points[lastPoint][j][k + 1].second ) );
				::MoveToEx( dc, round( points[lastPoint][j][k].first ), round( points[lastPoint][j][k].second ), NULL );
			}
		}
	}

	::DeleteObject(linePen);

	if( needToFillPolygons ) {
		fillWithGradient( dc, yPolygonPoints, zPolygonPoints );
	}
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
	std::vector< std::vector < std::vector< std::pair<double, double> > > > points = graphInPoints.getRelativePoints();
	text = std::to_string(graphInPoints.getGridSize());

	int pointsCount = 5;
	double maxValue = graphInPoints.getXMax();
	double axisScaleCoordUnit = maxValue / pointsCount;
	double axisScaleCoord = 0;
	std::pair<double, double> projectionCoord;
	for (int i = 0; i < pointsCount; ++i ) {
		axisScaleCoord += axisScaleCoordUnit;
		text = std::to_string(axisScaleCoord);
		projectionCoord = graphInPoints.getXProjection(axisScaleCoord);
		::TextOut(dc, projectionCoord.first, projectionCoord.second,
			(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length() - 4);
	}

	linePen = ::CreatePen(PS_SOLID, 1, RGB(200, 100, 200));
	::SelectObject(dc, linePen);
	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(yAxis.first * 10000000000) / 1000, round(yAxis.second * 10000000000) / 1000);

	text = "Y";
	::SetTextColor(dc, RGB(200, 100, 200));
	::TextOut(dc, round(origin.first + yAxis.first * 200), round(origin.second + yAxis.second * 200),
		(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length());
	maxValue = graphInPoints.getYMax();
	axisScaleCoordUnit = maxValue / pointsCount;
	axisScaleCoord = 0;
	for (int i = 0; i < pointsCount; ++i) {
		axisScaleCoord += axisScaleCoordUnit;
		text = std::to_string(axisScaleCoord);
		projectionCoord = graphInPoints.getYProjection(axisScaleCoord);
		::TextOut(dc, projectionCoord.first, projectionCoord.second,
			(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length() - 4);
	}

	linePen = ::CreatePen(PS_SOLID, 1, RGB(100, 200, 200));
	::SelectObject(dc, linePen);
	::MoveToEx(dc, round(origin.first), round(origin.second), NULL);
	::LineTo(dc, round(zAxis.first * 10000000000) / 1000, round(zAxis.second * 10000000000) / 1000);
	::DeleteObject(linePen);

	text = "Z";
	::SetTextColor(dc, RGB(100, 200, 200));
	::TextOut(dc, round(origin.first + zAxis.first * 200), round(origin.second + zAxis.second * 200),
		(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length());
	maxValue = graphInPoints.getZMax();
	axisScaleCoordUnit = maxValue / pointsCount;
	axisScaleCoord = 0;
	for (int i = 0; i < pointsCount; ++i) {
		axisScaleCoord += axisScaleCoordUnit;
		text = std::to_string(axisScaleCoord);
		projectionCoord = graphInPoints.getZProjection(axisScaleCoord);
		::TextOut(dc, projectionCoord.first, projectionCoord.second,
			(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length() - 4);
}
}

void GraphWindow::getMaxMinZAndRelativeGridKnots(double& min, double& max, int& xMin, int& yMin, int& xMax, int& yMax) {
	std::vector<std::vector<std::vector<double>>> zCoordinates = graphInPoints.getZcoordinates();

	xMax = 0;
	xMin = 0;
	yMax = 0;
	yMin = 0;

	for (int i = 0; i < zCoordinates.size(); ++i) {
		for (int j = 0; j < zCoordinates[i].size(); ++j) {
			if( zCoordinates[i][j].size() > 0 ) {
				max = zCoordinates[i][j][0];
				min = zCoordinates[i][j][0];
			}
		}
	}

	for( int i = 0; i < zCoordinates.size(); ++i ) {
		for( int j = 0; j < zCoordinates[i].size(); ++j ) {
			for( int k = 0; k < zCoordinates[i][j].size(); ++k ) {
				if( zCoordinates[i][j][k] < min ) {
					min = zCoordinates[i][j][k];
					xMin = i;
					yMin = j;
				}
				if( zCoordinates[i][j][k] > max ) {
					max = zCoordinates[i][j][k];
					xMax = i;
					yMax = j;
				}
			}
		}
	}
}

void GraphWindow::generatePointsOfMaxAndMinGradientColor( Gdiplus::Point &maxColorPoint, Gdiplus::Point &minColorPoint, 
														double& min, double& max, int& xMin, int& yMin, int& xMax, int& yMax )
{
	int semiGridSize = graphInPoints.getGridSize() / 2;

	std::pair< double, double > minPointPair = graphInPoints.getRelativePointWithXYZ( semiGridSize, semiGridSize, min - semiGridSize );
	std::pair< double, double > maxPointPair = graphInPoints.getRelativePointWithXYZ( semiGridSize, semiGridSize, max + semiGridSize );

	minColorPoint.X = (int) minPointPair.first;
	minColorPoint.Y = (int) minPointPair.second;
	maxColorPoint.X = (int) maxPointPair.first;
	maxColorPoint.Y = (int) maxPointPair.second;
}

void GraphWindow::fillWithGradient( HDC dc, std::vector< std::vector < std::vector< PointF > > >& yPolygonPoints, std::vector< std::vector < std::vector< PointF > > >& zPolygonPoints,
	Color maxColor, Color minColor )
{
	Graphics graphics( dc );
	graphics.SetInterpolationMode( InterpolationModeNearestNeighbor );
	graphics.SetSmoothingMode( SmoothingModeNone );
	graphics.SetPixelOffsetMode( PixelOffsetModeNone );
	graphics.SetCompositingQuality( CompositingQualityHighSpeed );
	graphics.SetTextRenderingHint( TextRenderingHintSingleBitPerPixel );

	int xMax, yMax, xMin, yMin;
	double max, min;
	getMaxMinZAndRelativeGridKnots( min, max, xMin, yMin, xMax, yMax );

	std::vector< Polygon4Wrap > polygons;
	
	for( int i = 0; i < yPolygonPoints.size( ) - 1; ++i ) {
		for( int k = 0; k < yPolygonPoints[i].size( ); ++k ) {
			for( int j = 0; j < ((yPolygonPoints[i + 1].size( ) > k) ? (min( yPolygonPoints[i][k].size( ), yPolygonPoints[i + 1][k].size( ) ) - 1) : 0); ++j ) {
				Polygon4Wrap wrap;
				wrap.poly[0] = yPolygonPoints[i][k][j];
				wrap.poly[1] = yPolygonPoints[i][k][j + 1];
				wrap.poly[2] = yPolygonPoints[i + 1][k][j + 1];
				wrap.poly[3] = yPolygonPoints[i + 1][k][j];

				polygons.push_back( wrap );
			}
		}
	}

	for( int i = 0; i < zPolygonPoints.size() - 1; ++i ) {
		for( int k = 0; k < zPolygonPoints[i].size( ); ++k ) {
			int sizeJ = min( zPolygonPoints[i][k].size(), zPolygonPoints[i + 1][k].size() ) - 1;
			for( int j = 0; j < ( (zPolygonPoints[i + 1].size() > k) ? sizeJ : 0 ); ++j ) {
				Polygon4Wrap wrap;
				wrap.poly[0] = zPolygonPoints[i][k][j];
				wrap.poly[1] = zPolygonPoints[i][k][j + 1];
				wrap.poly[2] = zPolygonPoints[i + 1][k][j + 1];
				wrap.poly[3] = zPolygonPoints[i + 1][k][j];

				polygons.push_back( wrap );
			}
		}
	}

	Point maxPoint, minPoint;
	generatePointsOfMaxAndMinGradientColor( maxPoint, minPoint, min, max, xMin, yMin, xMax, yMax );

	LinearGradientBrush linGrBrush(
		maxPoint,
		minPoint,
		maxColor,
		minColor);

	for (int i = 0; i < polygons.size(); ++i) {
		graphics.FillPolygon( &linGrBrush, polygons[i].poly, 4 );
	}
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