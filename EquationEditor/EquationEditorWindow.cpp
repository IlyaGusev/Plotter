#include "EquationEditorWindow.h"

const wchar_t* const CEquationEditorWindow::className = L"EquationEditorWindow";

CEquationEditorWindow::CEquationEditorWindow() : hwnd( nullptr ) {
	presenter = CEquationPresenter();
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

	HWND hwndEdit = ::CreateWindowEx( 0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL );

	originEditControlProc = (WNDPROC) ::SetWindowLong( hwndEdit, GWL_WNDPROC, (DWORD) editControlProc );

	presenter.AddControlView( new CEditControlView( hwndEdit ), NULL );
}

void CEquationEditorWindow::OnSize( int cxSize, int cySize ) {
}

void CEquationEditorWindow::OnChar() {
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(::GetWindowLong( hwnd, GWL_HINSTANCE ));

	HWND hwndEdit1 = ::CreateWindowEx( 0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL );

	::SetWindowLong( hwndEdit1, GWL_WNDPROC, (DWORD) editControlProc );

	HWND hwndEdit2 = ::CreateWindowEx( 0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL );

	::SetWindowLong( hwndEdit2, GWL_WNDPROC, (DWORD) editControlProc );

	presenter.AddControlView( new CFracControlView( this, new CEditControlView( hwndEdit1 ), new CEditControlView( hwndEdit2 ) ), ::GetFocus() );
}

void CEquationEditorWindow::OnDrawFrac( RECT rect ) {
	paintedLines.push_back( CLine( rect.left, (rect.top + rect.bottom) / 2, rect.right, (rect.top + rect.bottom) / 2 ) );
	::InvalidateRect( hwnd, NULL, FALSE );
}

void CEquationEditorWindow::OnDraw() {
	PAINTSTRUCT ps;
	//RECT rect;
	//::GetClientRect( hwnd, &rect );
	HDC hdc = ::BeginPaint( hwnd, &ps );
	//HDC backbuffDC = ::CreateCompatibleDC( hdc );
	//HBITMAP backbuffer = ::CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
	//::SelectObject( backbuffDC, backbuffer );
	//
	if( !paintedLines.empty() ) {
		for( CLine line : paintedLines ) {
			::MoveToEx( hdc, line.left, line.top, NULL );
			::LineTo( hdc, line.right, line.bottom );
		}
	}
	//::BitBlt( hdc, 0, 0, rect.right - rect.left, rect.top - rect.bottom, backbuffDC, 0, 0, SRCCOPY );

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
	}
	return ::DefWindowProc( handle, message, wParam, lParam );
}

LRESULT CALLBACK CEquationEditorWindow::editControlProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {
	CEquationEditorWindow *wnd = reinterpret_cast<CEquationEditorWindow*>(::GetWindowLong( ::GetParent( hwnd ), GWL_USERDATA ));

	switch( message ) {
	case WM_CHAR:
		wnd->OnChar();
	default:
		break;
	}
	return ::CallWindowProc( (WNDPROC) wnd->originEditControlProc, hwnd, message, wParam, lParam );
}