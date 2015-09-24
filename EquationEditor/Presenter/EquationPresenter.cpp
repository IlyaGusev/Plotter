#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView* newView )
{
	view = newView;

	CRectI rect(20, 20, 27, 40);

	root = std::shared_ptr<CExprControlModel>( new CExprControlModel( ) );
	root->SetRect( rect );

	caret.caretPoint.x = rect.Left();
	caret.caretPoint.y = rect.Top();
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
	auto drawingFuction = [=]( CTreeBfsProcessor::Node node )
	{
		CDrawParams curNodeDrawParams = node->GetDrawParams( );
		if( !curNodeDrawParams.polygon.empty() ) {
			view->DrawPolygon( hdc, curNodeDrawParams.polygon );
		}
		if( !curNodeDrawParams.text.empty() ) {
			view->DrawText( hdc, curNodeDrawParams.text, node->GetRect( ) );
		}
	};
	CTreeBfsProcessor drawer( root, drawingFuction );
	drawer.Process();

	view->SetCaret( caret.caretPoint, caret.curEdit->GetRect().Bottom() - caret.curEdit->GetRect().Top() );
}

std::pair<int, int> CEquationPresenter::findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x ) {
	int offset = 0;
	int length = editControlModel->GetRect().Left();
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
	auto predicate = [=]( CTreeBfsProcessor::Node node ) -> bool
	{
		return node->GetRect().isContain( x, y ) && node->GetType() == TEXT;
	};
	auto hint = [=]( CTreeBfsProcessor::Node node, CTreeBfsProcessor::Node child ) -> bool
	{
		return child->GetRect().isContain( x, y );
	};

	CTreeBfsProcessor processor( root );
	auto firstCandidate = processor.Find( predicate, hint );
	if( firstCandidate == nullptr ) {
		return;
	}
	if( caret.curEdit != firstCandidate ) {
		caret.curEdit = std::dynamic_pointer_cast<CEditControlModel>(firstCandidate);
	}

	std::pair<int, int> newCaretPos = findCaretPos( caret.curEdit, x );
	caret.caretPoint.x = newCaretPos.first;
	caret.offset = newCaretPos.second;
	caret.caretPoint.y = caret.curEdit->GetRect().Top();

	view->Redraw( );
}

void CEquationPresenter::setFracRects( CRectI parentRect, std::shared_ptr<CFracControlModel> fracModel ) {
	// Выставляем размеры вьюшек
	// Высота дроби - две высоты соседнего текстового поля
	CRectI fracRect, firstChildRect, secondChildRect;
	firstChildRect.Bottom() = fracRect.Bottom() = parentRect.Bottom() - (parentRect.Top() - parentRect.Bottom()) / 2;
	secondChildRect.Bottom() = (parentRect.Top() + parentRect.Bottom()) / 2;
	secondChildRect.Top() = fracRect.Top() = parentRect.Top() + (parentRect.Top() - parentRect.Bottom()) / 2;
	firstChildRect.Top() = (parentRect.Top() + parentRect.Bottom()) / 2;
	firstChildRect.Left() = secondChildRect.Left() = fracRect.Left() = caret.caretPoint.x;
	firstChildRect.Right() = secondChildRect.Right() = fracRect.Right() = caret.caretPoint.x + 15;

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
	CRectI rect = newEditControl->GetRect();
	rect.Left() += fracModel->GetRect().Right() - fracModel->GetRect().Left();
	rect.Right() += fracModel->GetRect().Right() - fracModel->GetRect().Left();
	newEditControl->SetRect( rect );
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange( fracModel );

	view->Redraw();
}


void CEquationPresenter::setDegrRects( CRectI parentRect, std::shared_ptr<CDegrControlModel> degrModel ) {
	// Выставляем размеры вьюшек
	// высота показателя - 3/4 высоты родительского; пересекается в 2/4 высоты родительского с основанием
	CRectI degrRect, childRect;
	degrRect.Bottom() = parentRect.Bottom();
	degrRect.Top() = (parentRect.Top() - ((parentRect.Bottom() - parentRect.Top()) / 4));
	childRect.Top() = (parentRect.Top() - ((parentRect.Bottom() - parentRect.Top()) / 4));
	childRect.Bottom() = (parentRect.Bottom() - ((parentRect.Bottom() - parentRect.Top()) / 2));
	childRect.Left() = degrRect.Left() = caret.caretPoint.x;
	childRect.Right() = degrRect.Right() = caret.caretPoint.x + 15;

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
	CRectI rect = newEditControl->GetRect();
	rect.Left() += (degrModel->GetRect().Right() - degrModel->GetRect().Left());
	rect.Right() += (degrModel->GetRect().Right() - degrModel->GetRect().Left());
	newEditControl->SetRect(rect);
	parent->AddChildAfter( newEditControl, degrModel );

	updateTreeAfterSizeChange( degrModel );

	view->Redraw( );
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
	// не подавайте сюда корень дерева, всё сломается
	auto node = startVert->GetParent();
	while (node->GetParent() != nullptr)
	{
		auto oldRect = node->GetRect();
		node->Resize();
		auto newRect = node->GetRect();
		node = node->GetParent();

		if (oldRect.Left() == newRect.Left()
			&& oldRect.Right() == newRect.Right()
			&& oldRect.Top() == newRect.Top()
			&& oldRect.Bottom() == newRect.Bottom())
		{
			break;
		}
	}

	std::function<void( std::shared_ptr<IBaseExprModel> )> permutateFunction( []( std::shared_ptr<IBaseExprModel> node )
	{
		node->PermutateChildren();
	} );

	CTreeBfsProcessor processor( node, permutateFunction );

	processor.Process();
}