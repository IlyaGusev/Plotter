#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView& newView ) : 
	view( newView )
{
	CRect rect(20, 20, 20, 40);

	root = std::make_shared<CExprControlModel>( CExprControlModel( rect, std::weak_ptr<IBaseExprModel>() ) );
	root->InitializeChildren();
	caret.SetCurEdit( root->GetChildren().front() );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) 
{
	int symbolWidth = view.GetCharWidth( symbol, caret.GetCurEdit()->GetRect().GetHeight() );
	caret.GetCurEdit()->InsertSymbol( symbol, caret.Offset(), symbolWidth );
	++caret.Offset();

	updateTreeAfterSizeChange( caret.GetCurEdit() );

	view.Redraw();
}

void CEquationPresenter::DeleteSymbol() 
{
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
		if( !node->GetLines().empty() ) {
			view.DrawPolygon( node->GetLines() );
		}
		if( !node->GetText().empty() ) {
			view.DrawText( node->GetText(), node->GetRect() );
		}
		if( node->IsHightlighted() ) {
			view.DrawHightlightedRect( node->GetRect() );
		}
	};
	CTreeBfsProcessor drawer( root, drawingFuction );
	drawer.Process();
	
	// Рисует каретку
	// +2 - чтобы был небольшой пробел между кареткой и символом
	view.SetCaret( caret.GetPointX() + 1, caret.GetPointY(), caret.GetHeight() );
}

std::pair<int, int> CEquationPresenter::findCaretPos( std::shared_ptr<CEditControlModel> editControlModel, int x ) 
{
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

void CEquationPresenter::SetCaret( int x, int y ) 
{
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
		caret.SetCurEdit( firstCandidate );
	}

	std::pair<int, int> newCaretPos = findCaretPos( caret.GetCurEdit(), x );
	caret.Offset() = newCaretPos.second;

	view.Redraw();
}

void CEquationPresenter::MoveCaretLeft() {
	caret.GetCurEdit()->GoLeft( caret.GetCurEdit(), caret );
	view.Redraw();
}

void CEquationPresenter::MoveCaretRight() {
	caret.GetCurEdit()->GoRight( caret.GetCurEdit(), caret );
	view.Redraw();
}

void CEquationPresenter::addFrac( std::shared_ptr<CExprControlModel> parent )
{
	// Создаем новые модели для дроби
	std::shared_ptr<CFracControlModel> fracModel( new CFracControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	fracModel->InitializeChildren();
	parent->AddChildAfter( fracModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	newEditControl->MoveBy( fracModel->GetRect().GetWidth(), 0 );
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange( fracModel );
	fracModel->SetRect( fracModel->GetRect() );		// Костыль: при обходе графа в PlaceChildren у детей еще не задано верное расположение

	view.Redraw();
}


void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent ) 
{

	std::shared_ptr<CDegrControlModel> degrModel(new CDegrControlModel(caret.GetCurEdit()->GetRect(), parent));
	degrModel->InitializeChildren();
	parent->AddChildAfter(degrModel, caret.GetCurEdit());

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl(caret.Offset());
	newEditControl->MoveBy(degrModel->GetRect().GetWidth(), 0);
	parent->AddChildAfter(newEditControl, degrModel);

	updateTreeAfterSizeChange(degrModel);
	degrModel->SetRect(degrModel->GetRect());		// Костыль: при обходе графа в PlaceChildren у детей еще не задано верное расположение

	view.Redraw();
}

void CEquationPresenter::addSubscript(std::shared_ptr<CExprControlModel> parent)
{

	std::shared_ptr<CSubscriptControlModel> subscriptModel(new CSubscriptControlModel(caret.GetCurEdit()->GetRect(), parent));
	subscriptModel->InitializeChildren();
	parent->AddChildAfter(subscriptModel, caret.GetCurEdit());

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl(caret.Offset());
	newEditControl->MoveBy(subscriptModel->GetRect().GetWidth(), 0);
	parent->AddChildAfter(newEditControl, subscriptModel);

	updateTreeAfterSizeChange(subscriptModel);
	subscriptModel->SetRect(subscriptModel->GetRect());		// Костыль: при обходе графа в PlaceChildren у детей еще не задано верное расположение

	view.Redraw();
}

void CEquationPresenter::AddControlView( ViewType viewType )
{
	// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
	// Родитель должен иметь тип CExprControlModel
	std::shared_ptr<CExprControlModel> parent( std::dynamic_pointer_cast<CExprControlModel>( caret.GetCurEdit()->GetParent().lock() ) );
	if( parent == nullptr ) {
		parent = root;
	}

	// Создаем новую вьюшку с выбранным типом
	switch( viewType ) {
	case FRAC:
		addFrac( std::shared_ptr<CExprControlModel>( parent ) );
		break;
	case DEGR:
		addDegr( std::shared_ptr<CExprControlModel>( parent ) );
		break;
	case SUBSCRIPT: 
		addSubscript(std::shared_ptr<CExprControlModel>(parent));
		break;
	default:
		break;
	}
}

	// не подавайте сюда корень дерева, всё сломается
void CEquationPresenter::updateTreeAfterSizeChange( std::shared_ptr<IBaseExprModel> startVert )
{
	auto node = startVert->GetParent().lock();
	while( node->GetParent().lock() != nullptr ) {
//		auto oldRect = node->Rect();
		node->Resize();
		node->PlaceChildren(); // скорее всего здесь спрятан костыль, но пока всё работает, а без него не очень. следует рассмотреть поподробнее случаи, когда здесь может всё сломаться
		
		// это условие нужно, чтобы не обходить лишний раз то, что никуда не переезжает. имеет смысл это доработать, чтобы ускорить работу
//		if( oldRect == node->Rect() ) {
//			break;
//		}

		node = node->GetParent().lock();
	}
	node->Resize();

	auto permutateFunction( []( CTreeBfsProcessor::Node n )
	{
		n->PlaceChildren();
	} );

	CTreeBfsProcessor processor( node, permutateFunction );

	processor.Process();
}