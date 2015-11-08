#include <vector>
#include <Windowsx.h>

#include "graphWindow.h"
#include "resource.h"
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

GraphWindow::GraphWindow( int width, int height, const wchar_t* formulaPath, bool is2D, bool isFillPolygonsIf3D ) :
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
		graphInPoints.changeScale( -2 );
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

	HDC hDc = CreateCompatibleDC(GetDC(handle));
    HBITMAP hBmp = CreateCompatibleBitmap(GetDC(handle), width, height);
    SelectObject(hDc, hBmp);
    BitBlt(hDc, 0, 0, width, height, GetDC(handle), 0, 0, SRCCOPY);
    BitmapToJpg(hBmp, width, height, filename);
    DeleteObject(hBmp);
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

	if (needToFillPolygons) {
		fillWithGradient( newHdc );
	}

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
		linePen = ::CreatePen( PS_SOLID, 1, RGB( 0, 10, 0 ) );
	} else {
		linePen = ::CreatePen( PS_SOLID, 1, RGB( 0, 255, 0 ) );
	}
	::SelectObject(dc, linePen);

	std::vector< std::vector < std::pair<double, double> > > points = graphInPoints.getRelativePoints();
	
	int pointsJSize = points[0].size() % 3 == 0 ? points[0].size() - 2 : 3 * (points[0].size() / 3) + 1;
	int pointsISize = points.size() % 3 == 0 ? points.size() - 2 : 3 * (points.size() / 3) + 1;
	for( size_t i = 0; i < pointsISize; ++i ) {
		POINT* lppoints = new POINT[pointsJSize];
		for( size_t j = 0; j < pointsJSize; ++j ) {
			lppoints[j] = { ( points[i][j].first ), ( points[i][j].second ) };
		}
		::PolyBezier( dc, lppoints, pointsJSize );

		delete[] lppoints;
	}

	for( size_t j = 0; j < pointsJSize; ++j ) {
		POINT* lppoints = new POINT[pointsISize];
		for( size_t i = 0; i < pointsISize; ++i ) {
			lppoints[i] = { ( points[i][j].first ),( points[i][j].second ) };
		}
		::PolyBezier( dc, lppoints, pointsISize );

		delete[] lppoints;
	}

	::DeleteObject(linePen);
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
	std::vector<std::vector<double>> zCoordinates = graphInPoints.getZcoordinates();

	xMax = 0;
	xMin = 0;
	yMax = 0;
	yMin = 0;
	max = zCoordinates[0][0];
	min = zCoordinates[0][0];

	for (int i = 0; i < zCoordinates.size(); ++i) {
		for (int j = 0; j < zCoordinates[i].size(); ++j) {
			if (zCoordinates[i][j] <= min) {
				min = zCoordinates[i][j];
				xMin = i;
				yMin = j;
			}
			if (zCoordinates[i][j] >= max) {
				xMax = i;
				yMax = j;
			}
		}
	}
}

void GraphWindow::getAllPolygonsOfGrid(std::vector< Polygon4Wrap > &polygons, std::vector< std::vector < std::pair<double, double> > > &points) {
	polygons.resize( 0 );

	for (size_t i = 0; i < points.size() - 1; ++i) {
		int firstSize = points[i].size() % 3 == 0 ? points[i].size() - 2 : 3 * (points[i].size() / 3) + 1;
		std::shared_ptr< std::vector< PointF > > firstPointsArray = std::shared_ptr< std::vector< PointF > >( new std::vector< PointF >( firstSize ) );
		for (size_t j = 0; j < firstSize; ++j) {
			(*firstPointsArray)[j] = PointF( points[i][j].first, points[i][j].second );
		}
		int secondSize = points[i + 1].size() % 3 == 0 ? points[i + 1].size() - 2 : 3 * (points[i + 1].size() / 3) + 1;
		std::shared_ptr< std::vector< PointF > > secondPointsArray = std::shared_ptr< std::vector< PointF > >( new std::vector< PointF >( secondSize ) );
		for (size_t j = 0; j < secondSize; ++j) {
			(*secondPointsArray)[j] = PointF( points[i + 1][j].first, points[i + 1][j].second );
		}

		int size = min( firstSize, secondSize );
		for (size_t t = 0; t < size - 1; ++t) {
			Polygon4Wrap wrap;
			wrap.poly[0] = (*firstPointsArray)[t];
			wrap.poly[1] = (*firstPointsArray)[t + 1];
			wrap.poly[2] = (*secondPointsArray)[t + 1];
			wrap.poly[3] = (*secondPointsArray)[t];

			polygons.push_back( wrap );
		}
	}
}

void GraphWindow::generatePointsOfMaxAndMinGradientColor( Gdiplus::Point &maxColorPoint, Gdiplus::Point &minColorPoint, 
														double& min, double& max, int& xMin, int& yMin, int& xMax, int& yMax, 
														std::vector< std::vector < std::pair<double, double> > > &points) 
{
	int semiGridSize = graphInPoints.getGridSize() / 2;

	std::pair< double, double > minPointPair = graphInPoints.getRelativePointWithXYZ( semiGridSize, semiGridSize, min - semiGridSize );
	std::pair< double, double > maxPointPair = graphInPoints.getRelativePointWithXYZ( semiGridSize, semiGridSize, max + semiGridSize );

	minColorPoint.X = (int)minPointPair.first;
	minColorPoint.Y = (int)minPointPair.second;
	maxColorPoint.X = (int)maxPointPair.first;
	maxColorPoint.Y = (int)maxPointPair.second;
}


void GraphWindow::fillWithGradient(HDC dc, Color maxColor, Color minColor) {
	Graphics graphics( dc );
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.SetSmoothingMode(SmoothingModeNone);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);
	graphics.SetCompositingQuality(CompositingQualityHighSpeed);
	graphics.SetTextRenderingHint(TextRenderingHintSingleBitPerPixel);

	int xMax, yMax, xMin, yMin;
	double max, min;
	getMaxMinZAndRelativeGridKnots( min, max, xMin, yMin, xMax, yMax );

	std::vector< Polygon4Wrap > polygons;
	std::vector< std::vector < std::pair<double, double> > > points = graphInPoints.getRelativePoints();
	getAllPolygonsOfGrid( polygons, points );

	Point maxPoint, minPoint;
	generatePointsOfMaxAndMinGradientColor(maxPoint, minPoint, min, max, xMin, yMin, xMax, yMax, points);

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
	int i = 0;
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

		case WM_SIZE:
			//that->OnSize(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_COMMAND :
			if (HIWORD(wParam) == 0) { //menu
				switch (LOWORD(wParam)) {
				case ID_FILE_SAVEIMAGE:
					that->OnImageSave();
					return 0;
				default:
					break;
				}
			}

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