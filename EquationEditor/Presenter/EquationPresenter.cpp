#include "EquationPresenter.h"

#include "EditControlModel.h"
#include "FracControlModel.h"

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
		int symbolWidth = view->GetCharWidth( caret.curEdit->GetDrawParams( ).text[caret.offset - 1] );
		caret.curEdit->DeleteSymbol( caret.offset - 1, symbolWidth );
		caret.caretPoint.x -= symbolWidth;
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
		for( IBaseExprModel* model : curNode->GetChilds() ) {
			queue.push( model );
		}
	}

	view->SetCaret( caret.caretPoint, caret.curEdit->GetRect().bottom - caret.curEdit->GetRect().top );
}
	
void CEquationPresenter::FindCaretPos( int x, int y ) {
	std::queue< IBaseExprModel* > queue;
	queue.push( root );

	while( !queue.empty() ) {
		IBaseExprModel* curNode = queue.front();
		queue.pop();
		
		RECT rect = curNode->GetRect();
		if( rect.left <= x && x <= rect.right ) {

		}

		// Если внутри
		for( IBaseExprModel* model : curNode->GetChilds() ) {
			queue.push( model );
		}
	}
}

//void CEquationPresenter::addFrac( IControlView* view, CExprControlModel* parent, RECT focusedViewRect ) {
//	CFracControlView* fracView = dynamic_cast<CFracControlView*>(view);
//
//	if( fracView != nullptr ) {
//		CFracControlModel* fracModel = new CFracControlModel();
//		CExprControlModel* firstChild = new CExprControlModel();
//		CEditControlModel* firstChildEdit = new CEditControlModel();
//		CExprControlModel* secondChild = new CExprControlModel();
//		CEditControlModel* secondChildEdit = new CEditControlModel();
//
//		// Выставляем размеры вьюшек
//		// Высота дроби - две высоты родителя
//		RECT fracRect, firstChildRect, secondChildRect;
//		firstChildRect.top = fracRect.top = focusedViewRect.top - (focusedViewRect.bottom - focusedViewRect.top) / 2;
//		secondChildRect.top = (focusedViewRect.bottom + focusedViewRect.top) / 2 + 1;
//		secondChildRect.bottom = fracRect.bottom = focusedViewRect.bottom + (focusedViewRect.bottom - focusedViewRect.top) / 2;
//		firstChildRect.bottom = (focusedViewRect.bottom + focusedViewRect.top) / 2 - 1;
//		firstChildRect.left = secondChildRect.left = fracRect.left = focusedViewRect.right;
//		firstChildRect.right = secondChildRect.right = fracRect.right = focusedViewRect.right + 7;
//
//		// Прикрепляем вьюшки к моделькам
//		//views[fracModel] = view;
//		//views[firstChild] = new CExprControlView();
//		//views[secondChild] = new CExprControlView();
//		//views[firstChildEdit] = fracView->GetFirstChild();
//		//views[secondChildEdit] = fracView->GetSecondChild();
//
//		// Посылаем размеры в модели и вьюшки
//		setRect( fracModel, fracRect );
//		setRect( firstChildEdit, firstChildRect );
//		setRect( secondChildEdit, secondChildRect );
//
//		// Обновляем граф
//		fracModel->SetParent( parent );
//		parent->AddChild( fracModel );
//		fracModel->SetFirstChild( firstChild );
//		firstChild->SetParent( fracModel );
//		fracModel->SetSecondChild( secondChild );
//		secondChild->SetParent( fracModel );
//		firstChild->AddChild( firstChildEdit );
//		firstChildEdit->SetParent( firstChild );
//		secondChild->AddChild( secondChildEdit );
//		secondChildEdit->SetParent( secondChild );
//	}
//}

void CEquationPresenter::AddControlView( ViewType viewType )
{
	IBaseExprModel* newModel;

	// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
	// Родитель должен иметь тип CExprControlModel
	CExprControlModel* parent = dynamic_cast<CExprControlModel*>(caret.curEdit->GetParent( ));
	if( parent == nullptr ) {
		parent = root;
	}

	// Создаем новую вьюшку с выбранным типом
	switch( viewType ) {
	case TEXT:
		newModel = new CEditControlModel();
		break;
	case FRAC:
		//addFrac( newView, parent, focusedModel->GetRect() );
		return;
	default:
		break;
	}

	newModel->SetRect( parent->GetRect() );
	parent->AddChild( newModel );
	newModel->SetParent( parent );
}