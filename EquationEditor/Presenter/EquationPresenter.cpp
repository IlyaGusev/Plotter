#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView* newView )
{
	view = newView;

	CRect rect(20, 20, 27, 40);

	root = std::shared_ptr<CExprControlModel>( new CExprControlModel( ) );
	root->SetRect( rect );

	caret.SetCurEdit( std::make_shared<CEditControlModel>( CEditControlModel( ) ) );
	caret.GetCurEdit()->SetRect( rect );

	root->AddChildAfter( caret.GetCurEdit(), nullptr );
	caret.GetCurEdit()->SetParent( root );
	caret.GetCurEdit()->SetRect( rect );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) {
	int symbolWidth = view->GetCharWidth( symbol );
	caret.GetCurEdit()->InsertSymbol( symbol, caret.Offset(), symbolWidth );
	++caret.Offset();

	updateTreeAfterSizeChange( caret.GetCurEdit() );

	view->Redraw();
}

void CEquationPresenter::DeleteSymbol() {
	if( caret.Offset() != 0 ) {
		caret.GetCurEdit()->DeleteSymbol( caret.Offset() - 1 );
		--caret.Offset();

		updateTreeAfterSizeChange( caret.GetCurEdit() );

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
			view->DrawText( hdc, curNodeDrawParams.text, node->Rect( ) );
		}
	};
	CTreeBfsProcessor drawer( root, drawingFuction );
	drawer.Process();

	view->SetCaret( caret.GetPointX(), caret.GetPointY(), caret.GetHeight() );
}

std::pair<int, int> CEquationPresenter::findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x ) {
	int offset = 0;
	int length = editControlModel->Rect().Left();
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
	auto predicate = [=]( CTreeBfsProcessor::Node node ) -> bool {
		return node->Rect().isContain( x, y ) && node->GetType() == TEXT;
	};
	auto hint = [=]( CTreeBfsProcessor::Node node, CTreeBfsProcessor::Node child ) -> bool {
		return child->Rect().isContain( x, y );
	};

	CTreeBfsProcessor processor( root );
	auto firstCandidate = processor.Find( predicate, hint );
	if( firstCandidate == nullptr ) {
		return;
	}
	if( caret.GetCurEdit() != firstCandidate ) {
		caret.SetCurEdit( std::dynamic_pointer_cast<CEditControlModel>(firstCandidate) );
	}

	std::pair<int, int> newCaretPos = findCaretPos( caret.GetCurEdit(), x );
	caret.Offset() = newCaretPos.second;

	view->Redraw( );
}

void CEquationPresenter::setFracRects( CRect parentRect, std::shared_ptr<CFracControlModel> fracModel ) {
	// Выставляем размеры вьюшек
	// Высота дроби - две высоты соседнего текстового поля
	CRect fracRect, firstChildRect, secondChildRect;
	firstChildRect.Bottom() = fracRect.Bottom() = parentRect.Bottom() - (parentRect.Top() - parentRect.Bottom()) / 2;
	secondChildRect.Bottom() = (parentRect.Top() + parentRect.Bottom()) / 2;
	secondChildRect.Top() = fracRect.Top() = parentRect.Top() + (parentRect.Top() - parentRect.Bottom()) / 2;
	firstChildRect.Top() = (parentRect.Top() + parentRect.Bottom()) / 2;
	firstChildRect.Left() = secondChildRect.Left() = fracRect.Left() = caret.GetPointX();
	firstChildRect.Right() = secondChildRect.Right() = fracRect.Right() = caret.GetPointX() + 15;

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
	setFracRects( caret.GetCurEdit()->Rect(), fracModel );

	// Обновляем граф
	fracModel->SetParent( parent );
	parent->AddChildAfter( fracModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	newEditControl->Rect().MoveBy( fracModel->Rect().Right() - fracModel->Rect().Left(), 0 );
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange( fracModel );

	view->Redraw();
}


void CEquationPresenter::setDegrRects( CRect parentRect, std::shared_ptr<CDegrControlModel> degrModel ) {
	// Выставляем размеры вьюшек
	// высота показателя - 3/4 высоты родительского; пересекается в 2/4 высоты родительского с основанием
	CRect degrRect, childRect;
	degrRect.Bottom() = parentRect.Bottom();
	degrRect.Top() = (parentRect.Top() - ((parentRect.Bottom() - parentRect.Top()) / 4));
	childRect.Top() = (parentRect.Top() - ((parentRect.Bottom() - parentRect.Top()) / 4));
	childRect.Bottom() = (parentRect.Bottom() - ((parentRect.Bottom() - parentRect.Top()) / 2));
	childRect.Left() = degrRect.Left() = caret.GetPointX();
	childRect.Right() = degrRect.Right() = caret.GetPointX() + 15;

	degrModel->SetRect(degrRect);
	degrModel->GetChildren().front()->SetRect(childRect);
	degrModel->GetChildren().front()->GetChildren().front()->SetRect(childRect);
}

void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent ) {
	// Создаем новые модели для степени
	std::shared_ptr<CDegrControlModel> degrModel = std::make_shared<CDegrControlModel>( CDegrControlModel() );

	// Посылаем размеры в модели
	setDegrRects(caret.GetCurEdit()->Rect(), degrModel);

	// Обновляем граф
	degrModel->SetParent(parent);
	parent->AddChildAfter( degrModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl(caret.Offset());
	CRect rect = newEditControl->Rect();
	rect.Left() += (degrModel->Rect().Right() - degrModel->Rect().Left());
	rect.Right() += (degrModel->Rect().Right() - degrModel->Rect().Left());
	newEditControl->SetRect(rect);
	parent->AddChildAfter( newEditControl, degrModel );

	updateTreeAfterSizeChange( degrModel );

	view->Redraw( );
}

void CEquationPresenter::AddControlView( ViewType viewType )
{
	// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
	// Родитель должен иметь тип CExprControlModel
	std::shared_ptr<CExprControlModel> parent( std::dynamic_pointer_cast<CExprControlModel>( caret.GetCurEdit()->GetParent() ) );
	if( parent == nullptr ) {
		parent = root;
	}

	// Создаем новую вьюшку с выбранным типом
	switch( viewType ) {
	case TEXT: {
		std::shared_ptr<IBaseExprModel> newModel( new CEditControlModel() );
		newModel->SetRect( parent->Rect() );
		parent->AddChildAfter( newModel, caret.GetCurEdit() );
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
	while( node->GetParent() != nullptr ) {
		auto oldRect = node->Rect();
		node->Resize();
		
		if( oldRect == node->Rect() ) {
			break;
		}

		node = node->GetParent();
	}

	std::function<void( std::shared_ptr<IBaseExprModel> )> permutateFunction( []( std::shared_ptr<IBaseExprModel> node ) {
		node->PermutateChildren();
	} );

	CTreeBfsProcessor processor( node, permutateFunction );

	processor.Process();
}