#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"
#include "Presenter/Utils/TreeDfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView& newView ) : 
	view( newView )
{
	CRect rect(20, 20, 27, 40);

	root = std::make_shared<CExprControlModel>( CExprControlModel( rect, std::weak_ptr<IBaseExprModel>() ) );
	root->InitializeChildren();
	caret.SetCurEdit( std::dynamic_pointer_cast<CEditControlModel>( root->GetChildren().front() ) );

	// initialize tree invalidate processors
	auto resizeFunction( []( CTreeDfsProcessor::Node n )
	{
		n->Resize( );
	} );
	resizeProcessor = CTreeDfsProcessor( root );
	resizeProcessor.SetExitProcessFunc( resizeFunction );

	auto placeFunction( []( CTreeBfsProcessor::Node n )
	{
		n->PlaceChildren( );
	} );

	placeProcessor = CTreeBfsProcessor( root, placeFunction );

	// initialize draw processor
	auto drawingFuction = [=]( CTreeBfsProcessor::Node node )
	{
		CDrawParams curNodeDrawParams = node->GetDrawParams( );
		if( !curNodeDrawParams.polygon.empty( ) ) {
			view.DrawPolygon( curNodeDrawParams.polygon );
		}
		if( !curNodeDrawParams.text.empty( ) ) {
			view.DrawText( curNodeDrawParams.text, node->GetRect( ) );
		}
	};
	drawer = CTreeBfsProcessor( root, drawingFuction );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) 
{
	int symbolWidth = view.GetCharWidth( symbol );
	caret.GetCurEdit()->InsertSymbol( symbol, caret.Offset(), symbolWidth );
	++caret.Offset();

	updateTreeAfterSizeChange();

	view.Redraw();
}

void CEquationPresenter::DeleteSymbol() 
{
	if( caret.Offset() != 0 ) {
		caret.GetCurEdit()->DeleteSymbol( caret.Offset() - 1 );
		--caret.Offset();

		updateTreeAfterSizeChange();

		view.Redraw();
	}
}

void CEquationPresenter::OnDraw() 
{
	drawer.Process();

	view.SetCaret( caret.GetPointX(), caret.GetPointY(), caret.GetHeight() );
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
		caret.SetCurEdit( std::dynamic_pointer_cast<CEditControlModel>( firstCandidate ) );
	}

	std::pair<int, int> newCaretPos = findCaretPos( caret.GetCurEdit(), x );
	caret.Offset() = newCaretPos.second;

	view.Redraw();
}

void CEquationPresenter::addFrac( std::shared_ptr<CExprControlModel> parent ) 
{
	// Создаем новые модели для дроби
	std::shared_ptr<CFracControlModel> fracModel( new CFracControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	fracModel->InitializeChildren();
	parent->AddChildAfter( fracModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange();

	view.Redraw();
}

void CEquationPresenter::setDegrRects(CRect parentRect, std::shared_ptr<CDegrControlModel> degrModel)
{
	// Выставляем размеры вьюшек
	// высота показателя - 3/4 высоты родительского; пересекается в 2/4 высоты родительского с основанием
	CRect degrRect;
	degrRect.Bottom() = parentRect.Bottom();
	degrRect.Top() = (parentRect.Top() - ((parentRect.Bottom() - parentRect.Top()) / 4));

	degrRect.Left() = caret.GetPointX();
	degrRect.Right() = caret.GetPointX() + 15;

	degrModel->SetRect(degrRect);

}

void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent ) 
{
	std::shared_ptr<CDegrControlModel> degrModel( new CDegrControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	
	degrModel->InitializeChildren();

	parent->AddChildAfter( degrModel, caret.GetCurEdit() );
	
	updateTreeAfterSizeChange( );
	//degrModel->SetRect(degrModel->GetRect());		// Костыль: при обходе графа в PlaceChildren у детей еще не задано верное расположение

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
	default:
		break;
	}
}

void CEquationPresenter::updateTreeAfterSizeChange()
{	
	resizeProcessor.Process();
	placeProcessor.Process();
}