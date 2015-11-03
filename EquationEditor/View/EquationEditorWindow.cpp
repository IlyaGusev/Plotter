#include <Windowsx.h>
#include "resource.h"
#include "View/EquationEditorWindow.h"

const wchar_t* const CEquationEditorWindow::className = L"EquationEditorWindow";

CEquationEditorWindow::CEquationEditorWindow() : hwnd( nullptr ) 
{
	presenter = std::make_shared<CEquationPresenter>(*this);
	isPressedShift = false;
  yMinScroll = 0;
  yCurrentScroll = 0;
  yMaxScroll = 0;
}

bool CEquationEditorWindow::RegisterClassW() 
{
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

bool CEquationEditorWindow::Create( HWND parent, RECT rect ) 
{
  symbolSelectedColorref = RGB( 0xFF, 0xFF, 0xFF );	// Белый
	symbolUnselectedColorref = RGB( 0, 0, 0 );			// Черный
	bkSelectedColorref = RGB( 0x1F, 0xAE, 0xE9 );		// Голубой
	bkUnselectedColorref = RGB( 0xFF, 0xFF, 0xFF );		// Белый
	bkSelectedHighlightColorref = RGB( 0x44, 0x4b, 0x52 );
	bkUnselectedHightlightColorref = RGB(0xF0, 0xF0, 0xF0);

  parent = parent;
	return ::CreateWindowEx( 0, className, L"Equation Editor", WS_CHILD | WS_EX_LAYERED | WS_CLIPCHILDREN | WS_VSCROLL, rect.left, rect.top, rect.right, rect.bottom,
		parent, nullptr, ::GetModuleHandle( nullptr ), this ) != 0;
}

void CEquationEditorWindow::Show( int cmdShow ) 
{
  UpdateScrollbar();
  ::ShowWindow( hwnd, cmdShow );
}

void CEquationEditorWindow::OnDestroy() 
{
	for( auto it = fonts.begin(); it != fonts.end(); ++it ) {
		::DeleteObject( it->second );
	}
	::PostQuitMessage( 0 );
}

void CEquationEditorWindow::OnCreate() 
{
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(::GetWindowLong( hwnd, GWL_HINSTANCE ));
	
	// Добавляем меню
//	HMENU hMenu = ::LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU1 ) );
//	::SetMenu( hwnd, hMenu );
}

void CEquationEditorWindow::UpdateScrollbar()
{
  RECT rect;
  GetClientRect(hwnd, &rect);
  int cySize = rect.bottom - rect.top;

  yMaxScroll = max(presenter->GetRoot()->GetRect().GetHeight() - cySize + 60, 0);
  yCurrentScroll = min(yCurrentScroll, yMaxScroll);

  SCROLLINFO si;
  si.cbSize = sizeof(si);
  si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
  si.nMin = yMinScroll;
  si.nMax = presenter->GetRoot()->GetRect().GetHeight() + 60;
  si.nPage = cySize;
  si.nPos = yCurrentScroll;
  BOOL res = SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

void CEquationEditorWindow::OnSize( int cxSize, int cySize ) 
{
}

void CEquationEditorWindow::Redraw() 
{
	RECT rctB = { 0, 0, 1800, 1800 };
	::InvalidateRect( hwnd, &rctB, TRUE );
  UpdateScrollbar();
}

int CEquationEditorWindow::GetSymbolWidth( wchar_t symbol, int symbolHeight ) 
{
	HDC hdc = GetDC( hwnd );
	HGDIOBJ oldObject = ::SelectObject( hdc, getFont(symbolHeight) );
	
	int symbolWidth = 0;
	::GetCharWidth32( hdc, symbol, symbol, &symbolWidth );
	::SelectObject( hdc, oldObject );
	::ReleaseDC( hwnd, hdc );
	return symbolWidth;
}

void CEquationEditorWindow::OnLButtonDown( int xMousePos, int yMousePos ) 
{
	presenter->SetCaret( xMousePos, yMousePos );
}

void CEquationEditorWindow::OnWmCommand( WPARAM wParam, LPARAM lParam ) 
{
	CCaret caret;
	if( HIWORD( wParam ) == 1 ) {
		switch( LOWORD( wParam ) ) {
			case ID_ACC_DEGR:
				presenter->AddControlView( DEGR );
				break;
			case ID_ACC_FRAC:
				presenter->AddControlView( FRAC );
				break;
			case ID_SHIFT_LEFT:
				caret = presenter->GetCaret();
				caret.GetCurEdit()->MoveCaretLeft( caret.GetCurEdit().get(), caret );
				presenter->SetSelection( caret.GetPointX(), caret.GetPointY() );
				break;
			case ID_SHIFT_RIGHT:
				caret = presenter->GetCaret();
				caret.GetCurEdit()->MoveCaretRight( caret.GetCurEdit().get(), caret );
				presenter->SetSelection( caret.GetPointX(), caret.GetPointY() );
				break;
			default:
				return;
		}
	} else if( HIWORD( wParam ) == 0 ) {
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
		case ID_ADD_PARANTHESIS:
			presenter->AddControlView( PARENTHESES );
			break;
		case ID_ADD_SUM:
			presenter->AddControlView( SUM );
			break;
		case ID_ADD_PRODUCT:
			presenter->AddControlView( PRODUCT );
			break;
		}
	}
}

void CEquationEditorWindow::OnKeyDown( WPARAM wParam ) 
{
	switch( wParam ) {
	case VK_DELETE:
		presenter->DeleteNextSymbol( GetKeyState( VK_CONTROL ) < 0 );
		break;

	case VK_BACK:
		presenter->DeleteSymbol( GetKeyState( VK_CONTROL ) < 0 );
		break;

	case VK_LEFT:   // LEFT ARROW
		presenter->MoveCaretLeft();
		break;

	case VK_RIGHT:  // RIGHT ARROW
		presenter->MoveCaretRight();
		break;

	case VK_UP:     // UP ARROW 
	case VK_DOWN:   // DOWN ARROW 
		presenter->Serialize();
		break;
	}
}

void CEquationEditorWindow::OnMouseMove( WPARAM wParam, int x, int y ) 
{
	if( wParam == MK_LBUTTON ) {
		presenter->SetSelection( x, y );
	}
}

void CEquationEditorWindow::OnScroll(WPARAM wParam)
{
  int xDelta = 0;
  int yDelta;     // yDelta = new_pos - current_pos 
  int yNewPos;    // new position 

  switch (LOWORD(wParam))
  {
    // User clicked the scroll bar shaft above the scroll box. 
  case SB_PAGEUP:
    yNewPos = yCurrentScroll - 50;
    break;

    // User clicked the scroll bar shaft below the scroll box. 
  case SB_PAGEDOWN:
    yNewPos = yCurrentScroll + 50;
    break;

    // User clicked the top arrow. 
  case SB_LINEUP:
    yNewPos = yCurrentScroll - 5;
    break;

    // User clicked the bottom arrow. 
  case SB_LINEDOWN:
    yNewPos = yCurrentScroll + 5;
    break;

    // User dragged the scroll box. 
  case SB_THUMBPOSITION:
    yNewPos = HIWORD(wParam);
    break;

  default:
    yNewPos = yCurrentScroll;
  }

  // New position must be between 0 and the screen height. 
  yNewPos = max(0, yNewPos);
  yNewPos = min(yMaxScroll, yNewPos);

  // If the current position does not change, do not scroll.
  if (yNewPos == yCurrentScroll)
    return;

  // Determine the amount scrolled (in pixels). 
  yDelta = yNewPos - yCurrentScroll;

  // Reset the current scroll position. 
  yCurrentScroll = yNewPos;

  ::ScrollWindowEx(hwnd, -xDelta, -yDelta, (CONST RECT *) NULL,
    (CONST RECT *) NULL, (HRGN)NULL, (PRECT)NULL,
    SW_INVALIDATE);
  ::UpdateWindow(hwnd);

  // Reset the scroll bar. 
  SCROLLINFO si;
  si.cbSize = sizeof(si);
  si.fMask = SIF_POS;
  si.nPos = yCurrentScroll;
  SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
}

void CEquationEditorWindow::OnChar( WPARAM wParam ) 
{
	switch( wParam ) {
	case 0x08:  // backspace
		return;
	case 0x0A:  // linefeed 
	case 0x0D:  // enter
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
		if( GetKeyState( VK_CONTROL ) < 0 ) {
			break;
		}
		presenter->InsertSymbol( (wchar_t) wParam );
		break;
	}
}

HFONT CEquationEditorWindow::getFont( int height ) 
{
	if( fonts[height] == 0 ) {
		fonts[height] = ::CreateFont( height, 0, 0, 0, 300, /*TRUE*/ false, false, false, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_STROKE_PRECIS, CLEARTYPE_QUALITY, /*FF_ROMAN*/ FF_SCRIPT, (LPCWSTR) /*"Cambria Math"*/ "Script" );
		// Cambria Math
	}
	return fonts[height];
}

void CEquationEditorWindow::DrawString( const std::wstring& text, const CRect& textRect, bool isSelected ) 
{
	RECT rect;
	rect.bottom = textRect.Bottom( );
	rect.top = textRect.Top( );
	rect.left = textRect.Left( );
	rect.right = textRect.Right( );
	HFONT font = getFont( textRect.GetHeight() );
	HGDIOBJ oldObject = ::SelectObject( hdc, font );
	
	if( isSelected ) {
		::SetTextColor( hdc, symbolSelectedColorref );
		::SetBkColor( hdc, bkSelectedColorref );
	} else {
		::SetTextColor( hdc, symbolUnselectedColorref );
		::SetBkColor( hdc, bkUnselectedColorref );
	}
	::DrawText( hdc, text.c_str(), text.size(), &rect, DT_LEFT );
	::SelectObject( hdc, oldObject );
}

void CEquationEditorWindow::DrawPolygon( const std::list<CLine>& polygon, bool isSelected ) 
{
	HGDIOBJ oldPen = ::SelectObject( hdc, ::GetStockObject( DC_PEN ) );
	if( isSelected ) {
		::SetDCPenColor( hdc, symbolSelectedColorref );
	} else {
		::SetDCPenColor( hdc, symbolUnselectedColorref );
	}
	if( !polygon.empty() ) {
		for( CLine line : polygon ) {
			::MoveToEx( hdc, line.StartX(), line.StartY(), nullptr );
			::LineTo( hdc, line.EndX(), line.EndY() );
		}
	}
	::SelectObject( hdc, oldPen );
}

void CEquationEditorWindow::DrawSelectedRect( const CRect& selectedRect ) 
{
	RECT rect;
	rect.bottom = selectedRect.Bottom();
	rect.top = selectedRect.Top();
	rect.left = selectedRect.Left();
	rect.right = selectedRect.Right();

	HBRUSH selectedHBrush = ::CreateSolidBrush( bkSelectedColorref );
	::FillRect( hdc, &rect, selectedHBrush );
	::DeleteObject( selectedHBrush );
}

void CEquationEditorWindow::DrawHighlightedRect( const CRect& controlRect, bool isSelected ) 
{
	HBRUSH highlightedHBrush = ::CreateSolidBrush( /*isSelected ? bkSelectedHighlightColorref :*/ bkUnselectedHightlightColorref );
	HBRUSH oldBrush = static_cast<HBRUSH>( ::SelectObject( hdc, highlightedHBrush ) );
	::Rectangle( hdc, controlRect.Left(), controlRect.Bottom(), controlRect.Right(), controlRect.Top() );
	::SelectObject( hdc, oldBrush );
	::DeleteObject( highlightedHBrush );
}

void CEquationEditorWindow::SetCaret( int caretPointX, int caretPointY, int height ) 
{
	::DestroyCaret();
	::CreateCaret( hwnd, (HBITMAP) nullptr, -1, height );
	::SetCaretPos( caretPointX, caretPointY );
	::ShowCaret( hwnd );
}

void CEquationEditorWindow::OnDraw() 
{
	PAINTSTRUCT ps;
	HDC curhdc = ::BeginPaint( hwnd, &ps );

	RECT rect;
	::GetClientRect( hwnd, &rect );
	hdc = ::CreateCompatibleDC( curhdc );
	HBITMAP backbuffer = ::CreateCompatibleBitmap( curhdc, rect.right - rect.left, rect.bottom - rect.top );
	::SelectObject( hdc, backbuffer );
	::FillRect( hdc, &rect, ::CreateSolidBrush( bkUnselectedColorref ) );
	
	presenter->OnDraw();

	::BitBlt( curhdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hdc, 0, 0, SRCCOPY );

	::DeleteObject( backbuffer );
	::DeleteDC( hdc );

	::EndPaint( hwnd, &ps );
}

LRESULT CEquationEditorWindow::equationEditorWindowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam ) 
{
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

	case WM_SIZE:
		wnd->OnSize( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_CHAR:
		wnd->OnChar( wParam );
		return 0;

	case WM_LBUTTONDOWN:
		wnd->OnLButtonDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
		return 0;

	case WM_MOUSEMOVE:
		wnd->OnMouseMove( wParam, GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
		return 0;

	case WM_COMMAND:
		wnd->OnWmCommand( wParam, lParam );
		return 0;

	case WM_KEYDOWN:
		wnd->OnKeyDown( wParam );
		return 0;

	case WM_ERASEBKGND:
		return 0;

  case WM_VSCROLL:
    wnd->OnScroll(wParam);
    return 0;
	}
	return ::DefWindowProc( handle, message, wParam, lParam );
}