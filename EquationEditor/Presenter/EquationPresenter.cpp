#include <queue>

#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeDfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView* newView )
{
	view = newView;

	RECT rect;
	rect.bottom = 40;
	rect.left = 20;
	rect.top = 20;
	rect.right = 27;

	root = std::shared_ptr<CExprControlModel>( new CExprControlModel( ) );
	root->SetRect( rect );

	caret.caretPoint.x = rect.left;
	caret.caretPoint.y = rect.top;
	caret.curEdit = std::make_shared<CEditControlModel>( CEditControlModel( ) );
	caret.curEdit->SetRect( rect );

	root->AddChildAfter( caret.curEdit, nullptr );
	caret.curEdit->SetParent( root );
	caret.curEdit->SetRect( rect );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) {
	int symbolWidth = view->GetCharWidth( symbol );
	caret.curEdit->InsertSymbol( symbol, caret.offset, symbolWidth );
	++caret.offset;
	caret.caretPoint.x += symbolWidth;

	updateTreeAfterSizeChange( caret.curEdit );

	view->Redraw();
}

void CEquationPresenter::DeleteSymbol() {
	if( caret.offset != 0 ) {
		caret.caretPoint.x -= caret.curEdit->DeleteSymbol( caret.offset - 1 );
		--caret.offset;

		updateTreeAfterSizeChange( caret.curEdit );

		view->Redraw();
	}
}

void CEquationPresenter::Draw( HDC hdc ) 
{
	std::queue< std::shared_ptr<IBaseExprModel> > queue;
	queue.push( root );
	
	while( !queue.empty() ) {
		std::shared_ptr<IBaseExprModel> curNode = queue.front( );
		queue.pop();

		CDrawParams curNodeDrawParams = curNode->GetDrawParams();
		if( !curNodeDrawParams.polygon.empty() ) {
			view->DrawPolygon( hdc, curNodeDrawParams.polygon );
		}
		if( !curNodeDrawParams.text.empty() ) {
			view->DrawText( hdc, curNodeDrawParams.text, curNode->GetRect( ) );
		}
		for( std::shared_ptr<IBaseExprModel> model : curNode->GetChildren( ) ) {
			queue.push( model );
		}
	}

	view->SetCaret( caret.caretPoint, caret.curEdit->GetRect().bottom - caret.curEdit->GetRect().top );
}

std::pair<int, int> CEquationPresenter::findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x ) {
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
	std::queue<std::shared_ptr<IBaseExprModel>> queue;
	for( std::shared_ptr<IBaseExprModel> child : root->GetChildren( ) ) {
		queue.push( std::shared_ptr<IBaseExprModel>( child ) );
	}
	std::shared_ptr<IBaseExprModel> curNode;
	while( !queue.empty() ) {
		curNode = queue.front();
		queue.pop();
		if( isInTheRect( x, y, curNode->GetRect() ) ) {
			if( curNode->GetType() == TEXT ) {
				if( caret.curEdit != curNode ) {
					caret.curEdit = std::dynamic_pointer_cast<CEditControlModel>(curNode);
				}
				std::pair<int, int> newCaretPos = findCaretPos( caret.curEdit, x );
				caret.caretPoint.x = newCaretPos.first;
				caret.offset = newCaretPos.second;
				caret.caretPoint.y = caret.curEdit->GetRect().top;
			} else {
				for( std::shared_ptr<IBaseExprModel> child : curNode->GetChildren() ) {
					queue.push( std::shared_ptr<IBaseExprModel>( child ) );
				}
			}
		}
	}
	view->Redraw( );
}

void CEquationPresenter::setFracRects( RECT parentRect, std::shared_ptr<CFracControlModel> fracModel ) {
	// Выставляем размеры вьюшек
	// Высота дроби - две высоты соседнего текстового поля
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

void CEquationPresenter::addFrac( std::shared_ptr<CExprControlModel> parent ) {
	// Создаем новые модели для дроби
	std::shared_ptr<CFracControlModel> fracModel( new CFracControlModel() );

	// Посылаем размеры в модели
	setFracRects( caret.curEdit->GetRect(), fracModel );

	// Обновляем граф
	fracModel->SetParent( parent );
	parent->AddChildAfter( fracModel, caret.curEdit );

	std::shared_ptr<CEditControlModel> newEditControl = caret.curEdit->SliceEditControl( caret.offset );
	RECT rect = newEditControl->GetRect();
	rect.left += fracModel->GetRect().right - fracModel->GetRect().left;
	rect.right += fracModel->GetRect().right - fracModel->GetRect().left;
	newEditControl->SetRect( rect );
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange( fracModel );

	view->Redraw();
}


void CEquationPresenter::setDegrRects( RECT parentRect, std::shared_ptr<CDegrControlModel> degrModel ) {
	// Выставляем размеры вьюшек
	// высота показателя - 3/4 высоты родительского; пересекается в 2/4 высоты родительского с основанием
	RECT degrRect, childRect;
	degrRect.bottom = parentRect.bottom;
	degrRect.top = (parentRect.top - ((parentRect.bottom - parentRect.top) / 4));
	childRect.top = (parentRect.top - ((parentRect.bottom - parentRect.top) / 4));
	childRect.bottom = (parentRect.bottom - ((parentRect.bottom - parentRect.top) / 2));
	childRect.left = degrRect.left = caret.caretPoint.x;
	childRect.right = degrRect.right = caret.caretPoint.x + 15;

	degrModel->SetRect(degrRect);
	degrModel->GetChildren().front()->SetRect(childRect);
	degrModel->GetChildren().front()->GetChildren().front()->SetRect(childRect);
}

void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent ) {
	// Создаем новые модели для степени
	std::shared_ptr<CDegrControlModel> degrModel = std::make_shared<CDegrControlModel>( CDegrControlModel() );

	// Посылаем размеры в модели
	setDegrRects(caret.curEdit->GetRect(), degrModel);

	// Обновляем граф
	degrModel->SetParent(parent);
	parent->AddChildAfter( degrModel, caret.curEdit );

	std::shared_ptr<CEditControlModel> newEditControl = caret.curEdit->SliceEditControl(caret.offset);
	RECT rect = newEditControl->GetRect();
	rect.left += (degrModel->GetRect().right - degrModel->GetRect().left);
	rect.right += (degrModel->GetRect().right - degrModel->GetRect().left);
	newEditControl->SetRect(rect);
	parent->AddChildAfter( newEditControl, degrModel );

	updateTreeAfterSizeChange( degrModel );

	view->Redraw( );
}


bool CEquationPresenter::isInTheRect( int x, int y, RECT rect ) {
	return rect.top <= y && y <= rect.bottom && rect.left <= x && x <= rect.right;
}

void CEquationPresenter::AddControlView( ViewType viewType )
{
	// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
	// Родитель должен иметь тип CExprControlModel
	std::shared_ptr<CExprControlModel> parent( std::dynamic_pointer_cast<CExprControlModel>( caret.curEdit->GetParent() ) );
	if( parent == nullptr ) {
		parent = root;
	}

	// Создаем новую вьюшку с выбранным типом
	switch( viewType ) {
	case TEXT: {
		std::shared_ptr<IBaseExprModel> newModel( new CEditControlModel() );
		newModel->SetRect( parent->GetRect() );
		parent->AddChildAfter( newModel, caret.curEdit );
		newModel->SetParent( parent );
		break;
	}
	case FRAC:
		addFrac( std::shared_ptr<CExprControlModel>( parent ) );
		break;
	case DEGR:
		addDegr( std::shared_ptr<CExprControlModel>( parent ) );
		break;
	default:
		break;
	}
}

void CEquationPresenter::updateTreeAfterSizeChange( std::shared_ptr<IBaseExprModel> startVert )
{
	auto node = startVert->GetParent();
	while (node->GetParent() != nullptr)
	{
		auto oldRect = node->GetRect();
		node->Resize();
		auto newRect = node->GetRect();
		node = node->GetParent();

		if (oldRect.left == newRect.left
			&& oldRect.right == newRect.right
			&& oldRect.top == newRect.top
			&& oldRect.bottom == newRect.bottom)
		{
			break;
		}
	}

	std::function<void( std::shared_ptr<IBaseExprModel> )> permutateFunction( []( std::shared_ptr<IBaseExprModel> node )
	{
		node->PermutateChildren();
	} );

	CTreeDfsProcessor processor( node, permutateFunction );

	processor.Process();
}