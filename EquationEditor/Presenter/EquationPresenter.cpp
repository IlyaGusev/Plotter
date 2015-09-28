#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView& newView ) : 
	view( newView )
{
	CRect rect(20, 20, 27, 40);

	root = std::make_shared<CExprControlModel>();
	root->SetRect( rect );
		
	caret.SetCurEdit( std::make_shared<CEditControlModel>( CEditControlModel( ) ) );
	caret.GetCurEdit()->SetRect( rect );

	root->AddChildAfter( caret.GetCurEdit(), nullptr );
	caret.GetCurEdit()->SetParent( root );
	caret.GetCurEdit()->SetRect( rect );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) {
	int symbolWidth = view.GetCharWidth( symbol );
	caret.GetCurEdit()->InsertSymbol( symbol, caret.Offset(), symbolWidth );
	++caret.Offset();

	updateTreeAfterSizeChange( caret.GetCurEdit() );

	view.Redraw();
}

void CEquationPresenter::DeleteSymbol() {
	if( caret.Offset() != 0 ) {
		caret.GetCurEdit()->DeleteSymbol( caret.Offset() - 1 );
		--caret.Offset();

		updateTreeAfterSizeChange( caret.GetCurEdit() );

		view.Redraw();
	}
}

void CEquationPresenter::OnDraw() 
{
	auto drawingFuction = [=]( CTreeBfsProcessor::Node node )
	{
		CDrawParams curNodeDrawParams = node->GetDrawParams( );
		if( !curNodeDrawParams.polygon.empty() ) {
			view.DrawPolygon( curNodeDrawParams.polygon );
		}
		if( !curNodeDrawParams.text.empty() ) {
			view.DrawText( curNodeDrawParams.text, node->GetRect( ) );
		}
	};
	CTreeBfsProcessor drawer( root, drawingFuction );
	drawer.Process();

	view.SetCaret( caret.GetPointX(), caret.GetPointY(), caret.GetHeight() );
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
	auto predicate = [=]( CTreeBfsProcessor::Node node ) -> bool {
		return node->GetRect().IsContain( x, y ) && node->GetType() == TEXT;
	};
	auto hint = [=]( CTreeBfsProcessor::Node child ) -> bool {
		return child->GetRect().IsContain( x, y );
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

	view.Redraw( );
}

void CEquationPresenter::setFracRects( CRect parentRect, std::shared_ptr<CFracControlModel> fracModel ) {
	// Выставляем размеры вьюшек
	// Ширина дроби - 15 пикселей
	// Высота дроби - две высоты соседнего текстового поля + 5
	CRect childRect = CRect( 0, 0, 15, parentRect.GetHeight() );
	fracModel->GetChildren().front()->SetRect( childRect );
	fracModel->GetChildren().front()->GetChildren().front()->SetRect( childRect );
	fracModel->GetChildren().back()->SetRect( childRect );
	fracModel->GetChildren().back()->GetChildren().front()->SetRect( childRect );
	fracModel->Resize();
	auto newRect = fracModel->GetRect();
	newRect.MoveBy( parentRect.Left(), parentRect.Top() );
	fracModel->SetRect( newRect );
	fracModel->PlaceChildren();
}

void CEquationPresenter::addFrac( std::shared_ptr<CExprControlModel> parent ) {
	// Создаем новые модели для дроби
	std::shared_ptr<CFracControlModel> fracModel( new CFracControlModel() );

	// Посылаем размеры в модели
	setFracRects( caret.GetCurEdit()->GetRect(), fracModel );

	// Обновляем граф
	fracModel->SetParent( parent );
	parent->AddChildAfter( fracModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	newEditControl->MoveBy( fracModel->GetRect().GetWidth(), 0 );
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange( fracModel );

	view.Redraw();
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
	setDegrRects(caret.GetCurEdit()->GetRect(), degrModel);

	// Обновляем граф
	degrModel->SetParent(parent);
	parent->AddChildAfter( degrModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl(caret.Offset());
	CRect rect = newEditControl->GetRect();
	rect.Left() += degrModel->GetRect().GetWidth();
	rect.Right() += degrModel->GetRect().GetWidth();
	newEditControl->SetRect(rect);
	parent->AddChildAfter( newEditControl, degrModel );

	updateTreeAfterSizeChange( degrModel );

	view.Redraw( );
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
		newModel->SetRect( parent->GetRect() );
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

	// не подавайте сюда корень дерева, всё сломается
void CEquationPresenter::updateTreeAfterSizeChange( std::shared_ptr<IBaseExprModel> startVert )
{
	auto node = startVert->GetParent();
	while( node->GetParent() != nullptr ) {
//		auto oldRect = node->Rect();
		node->Resize();
		node->PlaceChildren(); // скорее всего здесь спрятан костыль, но пока всё работает, а без него не очень. следует рассмотреть поподробнее случаи, когда здесь может всё сломаться
		
		// это условие нужно, чтобы не обходить лишний раз то, что никуда не переезжает. имеет смысл это доработать, чтобы ускорить работу
//		if( oldRect == node->Rect() ) {
//			break;
//		}

		node = node->GetParent();
	}
	node->Resize();

	auto permutateFunction( []( CTreeBfsProcessor::Node n )
	{
		n->PlaceChildren();
	} );

	CTreeBfsProcessor processor( node, permutateFunction );

	processor.Process();
}