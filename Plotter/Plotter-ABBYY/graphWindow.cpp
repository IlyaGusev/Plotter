#include <vector>
#include <Windowsx.h>
#include "resource.h"
#include "graphWindow.h"

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

const wchar_t* GraphWindow::nameClassWindow = L"ClassGraphWindow";
const wchar_t* GraphWindow::nameWindow = L"GraphWindow";

GraphWindow::GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D /*= false*/, bool isFillPolygonsIf3D /*= true */ ) :
	windowWidth(width),
	windowHeight(height),
	graphInPoints( formulaPath, is2D, 40 ),
	needToFillPolygons( isFillPolygonsIf3D )
{
}

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
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER | WS_CLIPCHILDREN,
		200, 20, windowWidth, windowHeight,
		NULL, NULL, hInstance, this);

	menu = ::LoadMenu(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));	// Загрузить меню из файла ресурса
	SetMenu(handle, menu);

	return handle;
}

void GraphWindow::Show() {
	::ShowWindow(handle, cmdShow);
	::UpdateWindow(handle);
}

HWND GraphWindow::GetHandle() {
	return handle;
}

void GraphWindow::OnCreate()
{
	HINSTANCE hInstance = (HINSTANCE) GetWindowLong( handle, GWL_HINSTANCE );

	plusButton = plusButtonImage = new Image( L"plus.png" );
	plusPressedButtonImage = new Image( L"plus_pressed.png" );
	plusRect = { 0, 0, 64, 64 };
	minusButton = minusButtonImage = new Image( L"minus.png" );
	minusPressedButtonImage = new Image( L"minus_pressed.png" );
	minusRect = { 0, 0, 64, 64 };
}

int GraphWindow::OnCommand( int loWord, int hiWord )
{
	if( hiWord == 0 ) { //menu
		switch( loWord ) {
		case ID_FILE_SAVEIMAGE:
			OnImageSave();
			return 0;
		default:
			break;
		}
	}
	return 0;
}

void GraphWindow::OnDestroy()
{
	::PostQuitMessage( 0 );

	delete plusButtonImage;
	delete plusPressedButtonImage;
	delete minusButtonImage;
	delete minusPressedButtonImage;
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
	if( !isInClickMode && wParam == MK_LBUTTON ) {
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
	if( plusRect.Contains( xMousePos, yMousePos ) ) {
		isInClickMode = true;
		plusButton = plusPressedButtonImage;
		::SendMessage( handle, WM_KEYDOWN, 0x58, 0 );
	} else if( minusRect.Contains( xMousePos, yMousePos ) ) {
		isInClickMode = true;
		minusButton = minusPressedButtonImage;
		::SendMessage( handle, WM_KEYDOWN, 0x5A, 0 );
	} else {
		prevMousePosX = xMousePos;
		prevMousePosY = yMousePos;
	}
}

void GraphWindow::OnLButtonUp( int xMousePos, int yMousePos )
{
	isInClickMode = false;
	plusButton = plusButtonImage;
	minusButton = minusButtonImage;
	::InvalidateRect( handle, NULL, FALSE );
}

//////////////////////////////

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;          // number of image encoders
    UINT size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
    {
        return -1;  // Failure
    }

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
    {
        return -1;  // Failure
    }

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }    
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

void BitmapToJpg(HBITMAP hbmpImage, int width, int height, LPWSTR filename)
{
    Bitmap *p_bmp = Bitmap::FromHBITMAP(hbmpImage, NULL);
    //Bitmap *p_bmp = new Bitmap(width, height, PixelFormat32bppARGB);
    
    CLSID pngClsid;
    int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
    /*if(result != -1)
        std::cout << "Encoder succeeded" << std::endl;
    else
        std::cout << "Encoder failed" << std::endl;*/
    p_bmp->Save(filename, &pngClsid, NULL);
    delete p_bmp;
}

bool GraphWindow::ScreenCapture(LPWSTR filename)
{
	RECT rect;
	GetClientRect( handle, &rect );
	int x = rect.left;
	int y = rect.top;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	HDC hDc = CreateCompatibleDC( GetDC( handle ) );
	HBITMAP hBmp = CreateCompatibleBitmap( GetDC( handle ), width, height );
	SelectObject( hDc, hBmp );
	BitBlt( hDc, 0, 0, width, height, GetDC( handle ), 0, 0, SRCCOPY );
	BitmapToJpg( hBmp, width, height, filename );
	DeleteObject( hBmp );
    return true;
}

//////////////////////////////

void GraphWindow::OnImageSave() {
	HANDLE file;
	wchar_t filename[256];
	filename[0] = '\0';
	OPENFILENAME enteredFileName;
	::ZeroMemory(&enteredFileName, sizeof(enteredFileName));
	enteredFileName.lStructSize = sizeof(OPENFILENAME);
	enteredFileName.hwndOwner = NULL;
	enteredFileName.lpstrFile = filename;
	enteredFileName.nMaxFile = 256;
	enteredFileName.lpstrFilter = L"Image Files( *.jpg )";
	enteredFileName.nFilterIndex = 1;
	enteredFileName.lpstrTitle = TEXT("Сохранить как");   //заголовок
	enteredFileName.lpstrInitialDir = NULL;   //начальный каталог для сохранения
	enteredFileName.Flags = OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT;

	if (::GetSaveFileName(&enteredFileName)) {
		ScreenCapture( enteredFileName.lpstrFile );
	}
}

void GraphWindow::OnSize(int width, int height) {
	graphInPoints.changeSize((width - 20) / 2, height / 2);

	plusRect.X = width - 64 - 10;
	minusRect.X = width - 64 - 10;
	plusRect.Y = height - 2 * 64 - 20;
	minusRect.Y = height - 64 - 10;
}

void GraphWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(handle, &ps);
	HDC newHdc = ::CreateCompatibleDC(hdc);
	RECT rect;
	::GetClientRect(handle, &rect);
	graphInPoints.changeSize((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

	HBITMAP bitmap = ::CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
	HGDIOBJ oldbitmap = ::SelectObject(newHdc, bitmap);

	drawGraph(newHdc);
	drawAxes(newHdc);
	Graphics graphics( newHdc );

	graphics.DrawImage( plusButton, plusRect );
	graphics.DrawImage( minusButton, minusRect );

	::BitBlt( hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY );

	::SelectObject(newHdc, oldbitmap);
	::DeleteObject(bitmap);

	::DeleteDC(hdc);
	::DeleteDC(newHdc);

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

	int iSize = points.size();
	int jSize = points[0].size();

	std::vector< std::vector < std::vector< PointF > > > yPolygonPoints( iSize );
	std::vector< std::vector< PointF > > zPolygonPoints( 2 * (iSize + jSize) );
	POINT* lppoints = new POINT[max( jSize, iSize )];

	// Проводим отрезки вдоль оси Y
	for( int i = 0; i < iSize; ++i ) {
		int j = 0;
		// Ищем, где точки начинаются
		for( ; j < jSize && points[i][j].size( ) == 0; ++j );
		// Считаем, что есть k слоев, рисуем отрезки послойно
		yPolygonPoints[i].resize( ((j < jSize) ? points[i][j].size() : 0) );
		for( int k = 0; k < ((j < jSize) ? points[i][j].size( ) : 0); ++k ) {
			int size = 0;
			int lastPoint = 0;
			// Проводим отрезок, соединяющий этот и следующий слой (вообще говоря, это нужно не всегда...)
			if( k + 1 < points[i][j].size() ) {
				::MoveToEx( dc, round( points[i][j][k].first ), round( points[i][j][k].second ), NULL );
				::LineTo( dc, round( points[i][j][k + 1].first ), round( points[i][j][k + 1].second ) );
				zPolygonPoints[i].push_back( PointF( points[i][j][k].first, points[i][j][k].second ) );
				zPolygonPoints[i].push_back( PointF( points[i][j][k + 1].first, points[i][j][k + 1].second ) );
			}
			for( int l = j; l < jSize; ++l ) {
				// Если в этой точке достаточно слоев - проводим отрезок через нее
				if( points[i][l].size() > k ) {
					// Если размер равен нулю и l != j - значит, среди отрезков на этой прямой был разрыв, заполняем его
					if( size == 0 && l != j ) {
						::LineTo( dc, round( points[i][l][k].first ), round( points[i][l][k].second ) );
					}
					lppoints[size] = { round( points[i][l][k].first ), round( points[i][l][k].second ) };
				
					yPolygonPoints[i][k].push_back( PointF( points[i][l][k].first, points[i][l][k].second ) );
					++size;
					lastPoint = l;
				} else {
					// Если слоев недостаточно, начинаем проводить кривые Безье по предыдущим точкам
					// Точек должно быть 3k+1
					int bezierPointsSize = size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1;
					::PolyBezier( dc, lppoints, bezierPointsSize );
					// Оставшиеся точки соединяем линией
					for( int p = bezierPointsSize - size; p < 0; ++p ) {
						if( l + p - 1 >= 0 && k < points[i][l + p - 1].size() && k < points[i][l + p].size() ) {
							::MoveToEx( dc, round( points[i][l + p - 1][k].first ), round( points[i][l + p - 1][k].second ), NULL );
							::LineTo( dc, round( points[i][l + p][k].first ), round( points[i][l + p][k].second ) );
							yPolygonPoints[i][k].push_back( PointF( points[i][l + p - 1][k].first, points[i][l + p - 1][k].second ) );
							yPolygonPoints[i][k].push_back( PointF( points[i][l + p][k].first, points[i][l + p][k].second ) );
						}
					}
					size = 0;
					if( l - 1 >= 0 && k < points[i][l - 1].size() ) {
						::MoveToEx( dc, round( points[i][l - 1][k].first ), round( points[i][l - 1][k].second ), NULL );
						yPolygonPoints[i][k].push_back( PointF( points[i][l - 1][k].first, points[i][l - 1][k].second ) );
					}
				}
			}

			::PolyBezier( dc, lppoints, size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1 );

			// Проводим отрезок в верхний слой
			if( k + 1 < points[i][lastPoint].size() ) {
				::MoveToEx( dc, round( points[i][lastPoint][k].first ), round( points[i][lastPoint][k].second ), NULL );
				zPolygonPoints[iSize + i].push_back( PointF( points[i][lastPoint][k].first, points[i][lastPoint][k].second ) );
				::LineTo( dc, round( points[i][lastPoint][k + 1].first ), round( points[i][lastPoint][k + 1].second ) );
				zPolygonPoints[iSize + i].push_back( PointF( points[i][lastPoint][k + 1].first, points[i][lastPoint][k + 1].second ) );
			}
		}
	}

	// Проводим отрезки вдоль оси X
	for( int j = 0; j < jSize; ++j ) {
		int i = 0;
		// Ищем, где точки начинаются
		for( ; i < iSize && points[i][j].size() == 0; ++i );
		// Считаем, что есть k слоев, рисуем отрезки послойно
		for( int k = 0; k < ((i < iSize) ? points[i][j].size( ) : 0); ++k ) {
			int size = 0;
			int lastPoint = 0;
			// Проводим отрезок, соединяющий этот и следующий слой (вообще говоря, это нужно не всегда...)
			if( k + 1 < points[i][j].size() ) {
				::MoveToEx( dc, round( points[i][j][k].first ), round( points[i][j][k].second ), NULL );
				::LineTo( dc, round( points[i][j][k + 1].first ), round( points[i][j][k + 1].second ) );
				zPolygonPoints[2 * iSize + j].push_back( PointF( points[i][j][k].first, points[i][j][k].second ) );
				zPolygonPoints[2 * iSize + j].push_back( PointF( points[i][j][k + 1].first, points[i][j][k + 1].second ) );
			}

			for( int l = i; l < iSize; ++l ) {
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
						if( l + p - 1 >= 0 && k < points[l + p - 1][j].size( ) && k < points[l + p][j].size( ) ) {
							::MoveToEx( dc, round( points[l + p - 1][j][k].first ), round( points[l + p - 1][j][k].second ), NULL );
							::LineTo( dc, round( points[l + p][j][k].first ), round( points[l + p][j][k].second ) );
						}
					}
					size = 0;
					if( l - 1 >= 0 && k < points[l - 1][j].size() ) {
						::MoveToEx( dc, round( points[l - 1][j][k].first ), round( points[l - 1][j][k].second ), NULL );
					}
				}
			}

			::PolyBezier( dc, lppoints, size % 3 == 0 ? size - 2 : 3 * (size / 3) + 1 );

			// Проводим отрезок в верхний слой
			if( k + 1 < points[lastPoint][j].size( ) ) {
				::MoveToEx( dc, round( points[lastPoint][j][k].first ), round( points[lastPoint][j][k].second ), NULL );
				zPolygonPoints[2 * iSize + jSize + j].push_back( PointF( points[lastPoint][j][k].first, points[lastPoint][j][k].second ) );
				::LineTo( dc, round( points[lastPoint][j][k + 1].first ), round( points[lastPoint][j][k + 1].second ) );
				zPolygonPoints[2 * iSize + jSize + j].push_back( PointF( points[lastPoint][j][k + 1].first, points[lastPoint][j][k + 1].second ) );
			}
		}
	}

	::DeleteObject(linePen);
	delete lppoints;

	if( needToFillPolygons ) {
		fillWithGradient( dc, yPolygonPoints, zPolygonPoints );
	}
}

void GraphWindow::drawAxes(HDC dc) {
	RECT rec;
	::GetClientRect(handle, &rec);

	HPEN linePen = ::CreatePen(PS_SOLID, 1, RGB(100, 100, 200));
	::SelectObject(dc, linePen);

	drawAxe(dc, 0, rec, "X");
	drawAxe(dc, 1, rec, "Y");

	drawAxe(dc, 2, rec, "Z");

	::DeleteObject(linePen);
}

void GraphWindow::drawAxe(HDC dc, int axisNum, RECT rec, const std::string axisName) {
	std::pair<double, double> axis = graphInPoints.getAxisVectorVisual(axisNum);
	std::pair<double, double> maxCoordinates = graphInPoints.getMaxRelativePoint(axis, rec.top, rec.bottom,
		rec.left, rec.right);
	std::pair<double, double> origin = graphInPoints.getOriginCoordinates();
	::MoveToEx(dc, (origin.first), (origin.second), NULL);
	::LineTo(dc, maxCoordinates.first, maxCoordinates.second);
	std::string text = axisName;
	::SetTextColor(dc, RGB(100, 100, 200));
	::TextOut(dc, maxCoordinates.first - axis.first * 20, maxCoordinates.second - axis.second * 20,
		(LPCWSTR)std::wstring(text.begin(), text.end()).c_str(), text.length());

	drawCoordinates(dc, axisNum, graphInPoints.getAxisMax(maxCoordinates, axis));
}

void GraphWindow::drawCoordinates(HDC dc, int axisNum, double maxValue, int pointsCount) {
	double axisScaleCoordUnit = maxValue / pointsCount;
	std::string text;
	double axisScaleCoord = 0;
	std::pair<double, double> projectionCoord;
	for (int i = 0; i < pointsCount - 1; ++i) {
		axisScaleCoord += axisScaleCoordUnit;
		text = std::to_string(axisScaleCoord);
		projectionCoord = graphInPoints.getProjection(axisScaleCoord, axisNum);
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

	std::pair< double, double > minPointPair = graphInPoints.getRelativePointWithXYZ( semiGridSize, semiGridSize, min - 2*semiGridSize );
	std::pair< double, double > maxPointPair = graphInPoints.getRelativePointWithXYZ( semiGridSize, semiGridSize, max + 2*semiGridSize );

	minColorPoint.X = (int) minPointPair.first;
	minColorPoint.Y = (int) minPointPair.second;
	maxColorPoint.X = (int) maxPointPair.first;
	maxColorPoint.Y = (int) maxPointPair.second;
}

void GraphWindow::fillWithGradient( HDC dc, std::vector< std::vector < std::vector< Gdiplus::PointF > > >& yPolygonPoints, std::vector < std::vector< Gdiplus::PointF > >& zPolygonPoints,
	Gdiplus::Color maxColor /*= Gdiplus::Color(128, 255, 0, 0)*/, Gdiplus::Color minColor /*= Gdiplus::Color(128, 0, 0, 255) */ )
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

	std::vector< CTriangle > polygons;
	
	for( int i = 0; i < yPolygonPoints.size() - 1; ++i ) {
		for( int k = 0; k < yPolygonPoints[i].size(); ++k ) {
			for( int j = 0; j + 1 < yPolygonPoints[i][k].size(); ++j )
			{
				CTriangle wrap;
				if( yPolygonPoints[i + 1].size() > k && yPolygonPoints[i + 1][k].size() > j ) {
					wrap.poly[0] = yPolygonPoints[i][k][j];
					wrap.poly[1] = yPolygonPoints[i][k][j + 1];
					wrap.poly[2] = yPolygonPoints[i + 1][k][j];
					polygons.push_back( wrap );
				}

				if( yPolygonPoints[i + 1].size() > k && yPolygonPoints[i + 1][k].size() > j + 1 ) {
					wrap.poly[0] = yPolygonPoints[i][k][j + 1];
					wrap.poly[1] = yPolygonPoints[i + 1][k][j + 1];
					wrap.poly[2] = yPolygonPoints[i + 1][k][j];
					polygons.push_back( wrap );
				}
			}
		}
	}

	int iSize = yPolygonPoints.size();
	int jSize = (zPolygonPoints.size() - 2 * iSize) / 2;
	std::vector<int> sizes = { 0, iSize, 2 * iSize, 2 * iSize + jSize, 2 * (iSize + jSize) };
	for( int k = 0; k < sizes.size() - 1; ++k ) {
		for( int i = sizes[k]; i < sizes[k+1]; ++i ) {
			for( int j = 0; j + 1 < zPolygonPoints[i].size(); ++j ) {
				CTriangle wrap;
				if( zPolygonPoints[i + 1].size() > j ) {
					wrap.poly[0] = zPolygonPoints[i][j];
					wrap.poly[1] = zPolygonPoints[i][j + 1];
					wrap.poly[2] = zPolygonPoints[i + 1][j];
					polygons.push_back( wrap );
				}

				if( zPolygonPoints[i + 1].size() > j + 1 ) {
					wrap.poly[0] = zPolygonPoints[i][j + 1];
					wrap.poly[1] = zPolygonPoints[i + 1][j + 1];
					wrap.poly[2] = zPolygonPoints[i + 1][j];
					polygons.push_back( wrap );
				}
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
		graphics.FillPolygon( &linGrBrush, polygons[i].poly, 3 );
	}
}

LRESULT __stdcall GraphWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_NCCREATE) {
		GraphWindow* that = reinterpret_cast<GraphWindow*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>(LPCREATESTRUCT( lParam )->lpCreateParams) );
		
		that->handle = handle;
		return ::DefWindowProc(handle, message, wParam, lParam);
	}
	int i = 0;
	GraphWindow* that = reinterpret_cast< GraphWindow* >(::GetWindowLong(handle, GWL_USERDATA));
	switch (message) {
		case WM_CREATE:
			that->OnCreate();
			return 0;

		case WM_COMMAND:
			return that->OnCommand( LOWORD( wParam ), HIWORD( wParam ) );

		case WM_CLOSE:
			that->OnClose();
			return 0;

		case WM_DESTROY:
			that->OnDestroy();
			return 0;

		case WM_PAINT:
			that->OnPaint();
			break;

		case WM_SIZE:
			that->OnSize(LOWORD(lParam), HIWORD(lParam));
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

		case WM_LBUTTONUP:
			that->OnLButtonUp( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;

		case WM_MOUSEMOVE:
			that->OnMouseMove( wParam, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			return 0;

		case WM_ERASEBKGND:
			return 1;
	}
		
	return ::DefWindowProc(handle, message, wParam, lParam);
}