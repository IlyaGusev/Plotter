#include "resource.h"

#include "View/EquationEditorWindow.h"

const wchar_t* const CEquationEditorWindow::className = L"EquationEditorWindow";

CEquationEditorWindow::CEquationEditorWindow() : hwnd( nullptr ) {
	presenter = std::shared_ptr<CEquationPresenter>( new CEquationPresenter( *this ) );
}

CEquationEditorWindow::~CEquationEditorWindow() {
}

bool CEquationEditorWindow::RegisterClassW() {
	WNDCLASSEX wnd;
	ZeroMemory( &wnd, sizeof(wnd) );
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = equationEditorWindowProc;
	wnd.hInstance = ::GetModuleHandle( NULL );
	wnd.lpszClassName = className;
	wnd.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wnd.hCursor = ::LoadCursor( NULL, IDC_ARROW );
	return ::RegisterClassEx( &wnd ) != 0;
}

bool CEquationEditorWindow::Create() {
	return ::CreateWindowEx( 0, className, L"Equation Editor", WS_OVERLAPPEDWINDOW | WS_EX_LAYERED, 0, 0, 500, 400,
		0, 0, ::GetModuleHandle( NULL ), this ) != 0;
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
	HMENU hMenu = ::LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU1 ) );
	::SetMenu( hwnd, hMenu );
}

void CEquationEditorWindow::OnSize( int cxSize, int cySize ) {
}

void CEquationEditorWindow::Redraw() {
	::InvalidateRect( hwnd, NULL, TRUE );
}

int CEquationEditorWindow::GetCharWidth( wchar_t symbol ) {
	HDC hdc = GetDC( hwnd );
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
			presenter->AddControlView(FRAC);
			break;
		case ID_ADD_DEGR:
			presenter->AddControlView(DEGR);
			break;
		}
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


void CEquationEditorWindow::DrawText( std::wstring text, CRect rectI ) {
	RECT rect;
	rect.bottom = rectI.Bottom();
	rect.top = rectI.Top();
	rect.left = rectI.Left();
	rect.right = rectI.Right();
	//HFONT hNewFont = ::CreateFont( rectI.Bottom() - rectI.Top(), 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
	//	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, (LPCWSTR) "Arial" );
	//HFONT hOldFont = (HFONT) ::SelectObject( hdc, hNewFont );

	::DrawText( hdc, text.c_str( ), text.size( ), &rect, DT_LEFT );

	//::SelectObject( hdc, hOldFont );
	//::DeleteObject( hNewFont );
}

void CEquationEditorWindow::DrawPolygon( std::list<CLine> polygon ) {
	if( !polygon.empty() ) {
		for( CLine line : polygon ) {
			::MoveToEx( hdc, line.StartX(), line.StartY(), NULL );
			::LineTo( hdc, line.EndX(), line.EndY() );
		}
	}
}

void CEquationEditorWindow::SetCaret( int caretPointX, int caretPointY, int height ) {
	::DestroyCaret();
	::CreateCaret( hwnd, (HBITMAP) NULL, -1, height );
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
		wnd->OnDestroy();
		return 0;

	case WM_CREATE:
		wnd->OnCreate();
		return 0;

	case WM_PAINT:
		wnd->OnDraw();
		return 0;
		//return ::DefWindowProc( handle, message, wParam, lParam );

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
	}
	return ::DefWindowProc( handle, message, wParam, lParam );
}