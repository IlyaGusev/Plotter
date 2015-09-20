#include "EquationPresenter.h"

#include <queue>

CEquationPresenter::CEquationPresenter( IEditorView* newView ) 
{
	view = newView;

	RECT rect;
	rect.bottom = 40;
	rect.left = 20;
	rect.top = 20;
	rect.right = 27;

	root = new CExprControlModel();
	root->SetRect( rect );

	caret.caretPoint.x = rect.left;
	caret.caretPoint.y = rect.top;
	caret.curEdit = new CEditControlModel();

	root->AddChild( caret.curEdit );
	caret.curEdit->SetParent( root );
	caret.curEdit->SetRect( rect );
}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) {
	int symbolWidth = view->GetCharWidth( symbol );
	caret.curEdit->InsertSymbol( symbol, caret.offset, symbolWidth );
	++caret.offset;
	caret.caretPoint.x += symbolWidth;
	// resetGraph();
	view->Redraw();
}

void CEquationPresenter::DeleteSymbol() {
	if( caret.offset != 0 ) {
		caret.caretPoint.x -= caret.curEdit->DeleteSymbol( caret.offset - 1 );
		--caret.offset;
		// resetGraph();
		view->Redraw();
	}
}

void CEquationPresenter::Draw( HDC hdc ) 
{
	std::queue< IBaseExprModel* > queue;
	queue.push( root );
	
	while( !queue.empty() ) {
		IBaseExprModel* curNode = queue.front();
		queue.pop();

		CDrawParams curNodeDrawParams = curNode->GetDrawParams();
		if( !curNodeDrawParams.polygon.empty() ) {
			view->DrawPolygon( hdc, curNodeDrawParams.polygon );
		}
		if( !curNodeDrawParams.text.empty() ) {
			view->DrawText( hdc, curNodeDrawParams.text, curNode->GetRect() );
		}
		for( IBaseExprModel* model : curNode->GetChildren() ) {
			queue.push( model );
		}
	}

	view->SetCaret( caret.caretPoint, caret.curEdit->GetRect().bottom - caret.curEdit->GetRect().top );
}

std::pair<int, int> CEquationPresenter::findCaretPos( CEditControlModel* editControlModel, int x ) {
	int offset = 0;
	int length = editControlModel->GetRect().left;
	for( int width : editControlModel->GetSymbolsWidths() ) {
		if( length >= x ) {
			break;
		}
		length += width;
		++offset;
	}
	return std::make_pair(length, offset);
}

void CEquationPresenter::SetCaret( int x, int y ) {
	std::queue<IBaseExprModel*> queue;
	queue.push( root );
	while( !queue.empty() ) {
		IBaseExprModel* curNode = queue.back();
		queue.pop();
		if( isInTheRect( x, y, curNode->GetRect() ) ) {
			CEditControlModel* editControlModel = dynamic_cast<CEditControlModel*>( curNode );
			if( editControlModel != nullptr ) {
				caret.curEdit = editControlModel;
				std::pair<int, int> newCaretPos = findCaretPos( editControlModel, x );
				caret.caretPoint.x = newCaretPos.first;
				caret.offset = newCaretPos.second;
				caret.caretPoint.y = editControlModel->GetRect().top;
			} else {
				for( IBaseExprModel* child : curNode->GetChildren() ) {
					queue.push( child );
				}
			}
		}
	}
	view->Redraw();
}

void CEquationPresenter::setFracRects( RECT parentRect, CFracControlModel* fracModel ) {
	// Выставляем размеры вьюшек
	// Высота дроби - две высоты родителя
	RECT fracRect, firstChildRect, secondChildRect;
	firstChildRect.bottom = fracRect.bottom = parentRect.bottom - (parentRect.top - parentRect.bottom) / 2;
	secondChildRect.bottom = (parentRect.top + parentRect.bottom) / 2;
	secondChildRect.top = fracRect.top = parentRect.top + (parentRect.top - parentRect.bottom) / 2;
	firstChildRect.top = (parentRect.top + parentRect.bottom) / 2;
	firstChildRect.left = secondChildRect.left = fracRect.left = caret.caretPoint.x;
	firstChildRect.right = secondChildRect.right = fracRect.right = caret.caretPoint.x + 15;

	fracModel->SetRect( fracRect );
	fracModel->GetChildren().front()->SetRect( firstChildRect );
	fracModel->GetChildren().front()->GetChildren().front()->SetRect( firstChildRect );
	fracModel->GetChildren().back()->SetRect( secondChildRect );
	fracModel->GetChildren().back()->GetChildren().front()->SetRect( secondChildRect );
}

void CEquationPresenter::addFrac( CExprControlModel* parent ) {
	// Создаем новые модели для дроби
	CFracControlModel* fracModel = new CFracControlModel();

	// Посылаем размеры в модели
	setFracRects( caret.curEdit->GetRect(), fracModel );

	// Обновляем граф
	fracModel->SetParent( parent );
	parent->AddChild( fracModel );

	CEditControlModel* newEditControl = caret.curEdit->SliceEditControl( caret.offset );
	RECT rect = newEditControl->GetRect();
	rect.left += fracModel->GetRect().right - fracModel->GetRect().left;
	rect.right += fracModel->GetRect().right - fracModel->GetRect().left;
	newEditControl->SetRect( rect );
	parent->AddChild( newEditControl );

	view->Redraw();
}

bool CEquationPresenter::isInTheRect( int x, int y, RECT rect ) {
	return rect.top <= y && y <= rect.bottom && rect.left <= x && x <= rect.right;
}

void CEquationPresenter::AddControlView( ViewType viewType )
{
	IBaseExprModel* newModel;

	// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
	// Родитель должен иметь тип CExprControlModel
	CExprControlModel* parent = dynamic_cast<CExprControlModel*>( caret.curEdit->GetParent() );
	if( parent == nullptr ) {
		parent = root;
	}

	// Создаем новую вьюшку с выбранным типом
	switch( viewType ) {
	case TEXT:
		newModel = new CEditControlModel();
		newModel->SetRect( parent->GetRect() );
		parent->AddChild( newModel );
		newModel->SetParent( parent );
		break;
	case FRAC:
		addFrac( parent );
		break;
	default:
		break;
	}
}