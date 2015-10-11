#include "resource.h"
#include "../RibbonFramework.h"
#include "View/EquationEditorWindow.h"

const wchar_t* const CEquationEditorWindow::className = L"EquationEditorWindow";

CEquationEditorWindow::CEquationEditorWindow() : hwnd( nullptr ) {
	presenter = std::make_shared<CEquationPresenter>(*this);
}

CEquationEditorWindow::~CEquationEditorWindow() {
	for( auto it = fonts.begin(); it != fonts.end(); ++it ) {
		::DeleteObject( it->second );
	}
}

bool CEquationEditorWindow::RegisterClassW() {
	WNDCLASSEX wnd;
	ZeroMemory( &wnd, sizeof(wnd) );
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = equationEditorWindowProc;
	wnd.hInstance = ::GetModuleHandle( nullptr );
	wnd.lpszClassName = className;
	wnd.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wnd.hCursor = ::LoadCursor( nullptr, IDC_ARROW );
	return ::RegisterClassEx( &wnd ) != 0;
}

bool CEquationEditorWindow::Create() {
	return ::CreateWindowEx( 0, className, L"Equation Editor", WS_OVERLAPPEDWINDOW | WS_EX_LAYERED, 0, 0, 500, 400,
		nullptr, nullptr, ::GetModuleHandle( nullptr ), this ) != 0;
}

void CEquationEditorWindow::Show( int cmdShow ) {
	::ShowWindow( hwnd, cmdShow );
}

void CEquationEditorWindow::OnDestroy() {
	::PostQuitMessage( 0 );
}

void CEquationEditorWindow::OnCreate() {
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(::GetWindowLong( hwnd, GWL_HINSTANCE ));
	
	// Добавляем меню
//	HMENU hMenu = ::LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU1 ) );
//	::SetMenu( hwnd, hMenu );
}

void CEquationEditorWindow::OnSize( int cxSize, int cySize ) {
}

void CEquationEditorWindow::Redraw() {
	RECT rctB = { 0, 180, 1800, 1800 };
	::InvalidateRect( hwnd, &rctB, TRUE );
}

int CEquationEditorWindow::GetSymbolWidth( wchar_t symbol, int symbolHeight ) {
	HDC hdc = GetDC( hwnd );
	::SelectObject( hdc, getFont(symbolHeight) );
	
	int symbolWidth = 0;
	::GetCharWidth32( hdc, symbol, symbol, &symbolWidth );
	::ReleaseDC( hwnd, hdc );
	return symbolWidth;
}

void CEquationEditorWindow::OnLButtonDown( int xMousePos, int yMousePos ) {
	presenter->SetCaret( xMousePos, yMousePos );
}

void CEquationEditorWindow::OnWmCommand( WPARAM wParam, LPARAM lParam ) {
	if( HIWORD( wParam ) == 0 ) {
		switch( LOWORD( wParam ) ) {
		case ID_ADD_FRAC:
			presenter->AddControlView( FRAC );
			break;
		case ID_ADD_DEGR:
			presenter->AddControlView( DEGR );
			break;
		case ID_ADD_SUBSCRIPT:
			presenter->AddControlView( SUBSCRIPT );
			break;
		case ID_ADD_RADICAL:
			presenter->AddControlView( RADICAL );
			break;
		}
	}
}

void CEquationEditorWindow::OnKeyDown( WPARAM wParam ) {
	switch( wParam ) {
	case VK_LEFT:   // LEFT ARROW 
		presenter->MoveCaretLeft();
		break;

	case VK_RIGHT:  // RIGHT ARROW
		presenter->MoveCaretRight();
		break;

	case VK_UP:     // UP ARROW 
	case VK_DOWN:   // DOWN ARROW 
		break;
	}
}

void CEquationEditorWindow::OnChar( WPARAM wParam ) {
	switch( wParam ) {
	case 0x08:  // backspace
		presenter->DeleteSymbol();
		return;
	case 0x0A:  // linefeed 
	case 0x1B:  // escape 
		MessageBeep( (UINT) -1 );
		return;

	case 0x09:  // tab 
		// Convert tabs to four consecutive spaces. 
		for( int i = 0; i < 4; ++i ) {
			::SendMessage( hwnd, WM_CHAR, 0x20, 0 );
		}
		return;

	default:    // displayable character
		presenter->InsertSymbol( (wchar_t) wParam );
		break;
	}
}

HFONT CEquationEditorWindow::getFont( int height ) {
	if( fonts[height] == 0 ) {
		fonts[height] = ::CreateFont( height, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, (LPCWSTR) "Courier New" );
	}
	return fonts[height];
}

void CEquationEditorWindow::DrawString( const std::wstring& text, const CRect& textRect ) {
	RECT rect;
	rect.bottom = textRect.Bottom( );
	rect.top = textRect.Top( );
	rect.left = textRect.Left( );
	rect.right = textRect.Right( );
	HFONT font = getFont( textRect.GetHeight() );
	::SelectObject( hdc, font );

	::DrawText( hdc, text.c_str(), text.size(), &rect, DT_LEFT );
}

void CEquationEditorWindow::DrawPolygon( const std::list<CLine>& polygon ) {
	if( !polygon.empty() ) {
		for( CLine line : polygon ) {
			::MoveToEx( hdc, line.StartX(), line.StartY(), nullptr );
			::LineTo( hdc, line.EndX(), line.EndY() );
		}
	}
}

void CEquationEditorWindow::DrawHightlightedRect( const CRect& controlRect ) {
	HBRUSH hightlightedHBrush = ::CreateSolidBrush( RGB( 0xF0, 0xF0, 0xF0 ) );
	HBRUSH oldBrush = static_cast<HBRUSH>( ::SelectObject( hdc, hightlightedHBrush ) );
	::Rectangle( hdc, controlRect.Left(), controlRect.Bottom(), controlRect.Right(), controlRect.Top() );
	::SelectObject( hdc, oldBrush );
	::DeleteObject( hightlightedHBrush );
}

void CEquationEditorWindow::SetCaret( int caretPointX, int caretPointY, int height ) {
	::DestroyCaret();
	::CreateCaret( hwnd, (HBITMAP) nullptr, -1, height );
	::SetCaretPos( caretPointX, caretPointY );
	::ShowCaret( hwnd );
}

void CEquationEditorWindow::OnDraw() {
	PAINTSTRUCT ps;
	hdc = ::BeginPaint( hwnd, &ps );

	presenter->OnDraw();
	//RECT rect;
	//::GetClientRect( hwnd, &rect );
	//HDC backbuffDC = ::CreateCompatibleDC( hdc );
	//HBITMAP backbuffer = ::CreateCompatibleBitmap( hdc, rect.endX - rect.startX, rect.endY - rect.startY );
	//::SelectObject( backbuffDC, backbuffer );

	
	//::BitBlt( hdc, 0, 0, rect.endX - rect.startX, rect.endY - rect.startY, backbuffDC, 0, 0, SRCCOPY );

	//::DeleteObject( backbuffer );
	//::DeleteDC( backbuffDC );
	::EndPaint( hwnd, &ps );
}

LRESULT CEquationEditorWindow::equationEditorWindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam ) {
	CEquationEditorWindow *wnd = nullptr;

	if( message == WM_NCCREATE ) {
		// Получаем указатель на экземпляр нашего окна, который мы передали в функцию CreateWindowEx
		wnd = static_cast<CEquationEditorWindow*>(LPCREATESTRUCT( lParam )->lpCreateParams);
		// И сохраняем в поле GWL_USERDATA
		::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>(LPCREATESTRUCT( lParam )->lpCreateParams) );
		// Запоминаем handle
		wnd->hwnd = handle;
	}
	// Теперь получаем указатель на наш экземпляр окна, но уже из поля GWL_USERDATA
	wnd = reinterpret_cast<CEquationEditorWindow*>(::GetWindowLong( handle, GWL_USERDATA ));

	switch( message ) {
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
		wnd->OnSize( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_CHAR:
		wnd->OnChar( wParam );
		return 0;

	case WM_LBUTTONDOWN:
		wnd->OnLButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_COMMAND:
		wnd->OnWmCommand( wParam, lParam );
		return 0;

	case WM_KEYDOWN:
		wnd->OnKeyDown( wParam );
	}
	return ::DefWindowProc( handle, message, wParam, lParam );
}