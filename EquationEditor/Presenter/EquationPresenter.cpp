#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"
#include "Presenter/Utils/TreeDfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView& newView ) : 
	view( newView )
{
	CRect rect(20, 20, 30, 40);

	root = std::make_shared<CExprControlModel>( CExprControlModel( rect, std::weak_ptr<IBaseExprModel>() ) );
	root->InitializeChildren();
	caret.SetCurEdit( root->GetChildren().front() );

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
		if( !node->GetLines( ).empty( ) ) {
			view.DrawPolygon( node->GetLines( ) );
		}
		if( !node->GetText( ).empty( ) ) {
			view.DrawString( node->GetText( ), node->GetRect( ) );
		}
		if( node->IsHightlighted( ) ) {
			view.DrawHightlightedRect( node->GetRect( ) );
		}
	};
	drawer = CTreeBfsProcessor( root, drawingFuction );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) 
{
	int symbolWidth = view.GetSymbolWidth( symbol, caret.GetCurEdit()->GetRect().GetHeight() );
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
	auto drawingFuction = [=]( CTreeBfsProcessor::Node node )
	{
		if( !node->GetLines().empty() ) {
			view.DrawPolygon( node->GetLines() );
		}
		if( !node->GetText().empty() ) {
			view.DrawString( node->GetText(), node->GetRect() );
		}
		if( node->IsHightlighted() ) {
			view.DrawHightlightedRect( node->GetRect() );
		}
	};
	CTreeBfsProcessor drawer( root, drawingFuction );
	drawer.Process();
	
	// Рисует каретку
	// +1 - чтобы был небольшой пробел между кареткой и символом
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
	parent->AddChildAfter( newEditControl, fracModel );

	updateTreeAfterSizeChange();

	view.Redraw();
}

void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent ) 
{
	std::shared_ptr<CDegrControlModel> degrModel( new CDegrControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	degrModel->InitializeChildren();
	parent->AddChildAfter( degrModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit( )->SliceEditControl( caret.Offset( ) );
	parent->AddChildAfter( newEditControl, degrModel );

	updateTreeAfterSizeChange();
	
	view.Redraw();
}

void CEquationPresenter::addSubscript(std::shared_ptr<CExprControlModel> parent)
{
	std::shared_ptr<CSubscriptControlModel> subscriptModel( new CSubscriptControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	subscriptModel->InitializeChildren();
	parent->AddChildAfter( subscriptModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, subscriptModel );

	updateTreeAfterSizeChange();
	
	view.Redraw();
}

void CEquationPresenter::addRadical(std::shared_ptr<CExprControlModel> parent)
{

	std::shared_ptr<CRadicalControlModel> radicalModel( new CRadicalControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	radicalModel->InitializeChildren();
	parent->AddChildAfter( radicalModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, radicalModel );

	updateTreeAfterSizeChange();
	
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
	case RADICAL:
		addRadical(std::shared_ptr<CExprControlModel>(parent));
	default:
		break;
	}
}

void CEquationPresenter::updateTreeAfterSizeChange()
{	
	resizeProcessor.Process();
	placeProcessor.Process();
}