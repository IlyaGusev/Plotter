#include "Presenter/EquationPresenter.h"
#include "Presenter/Utils/TreeBfsProcessor.h"
#include "Presenter/Utils/TreeDfsProcessor.h"

CEquationPresenter::CEquationPresenter( IEditorView& newView ) : 
	view( newView )
{
	CRect rect(20, 20, 30, 40);

	root = std::make_shared<CExprControlModel>( rect, std::weak_ptr<IBaseExprModel>() );
	root->InitializeChildren();
	caret.SetCurEdit( root->GetChildren().front() );

	// initialize tree invalidate processors
	highlightingProcessor = CTreeBfsProcessor( root, [] ( CTreeBfsProcessor::Node node ) {
		// Если у exprControl единственный ребенок - это текст. Если текст пустой - соответствующий editControl надо подсветить
		if( node->GetType() == EXPR ) {
			if( node->IsEmpty() ) {
				node->GetChildren().front()->HighlightingOn();
			} else {
				for( auto child : node->GetChildren() ) {
					child->HighlightingOff();
				}
			}
		}
	} );

	resizeProcessor = CTreeDfsProcessor( root );
	resizeProcessor.SetExitProcessFunc( [] ( CTreeDfsProcessor::Node node ) {
		node->Resize();
	} );

	placeProcessor = CTreeBfsProcessor( root, [] ( CTreeBfsProcessor::Node node ) {
		node->PlaceChildren();
	} );

	// initialize draw processor
	drawProcessor = CTreeBfsProcessor( root, [=] ( CTreeBfsProcessor::Node node ) {
		if( node->IsSelected() ) {
			view.DrawSelectedRect( node->GetRect() );
		}
		if( !node->GetLines().empty() ) {
			view.DrawPolygon( node->GetLines(), node->IsSelected() );
		}
		for( std::pair<std::wstring, CRect> selectedText : node->GetSelectedText() ) {
			view.DrawString( selectedText.first, selectedText.second, true );
		}
		for( std::pair<std::wstring, CRect> selectedText : node->GetUnselectedText() ) {
			view.DrawString( selectedText.first, selectedText.second, false );
		}
		if( node->IsHighlighted() ) {
			view.DrawHighlightedRect( node->GetRect(), node->IsSelected() );
		}
	} );

	deleteSelectionProcessor = CTreeBfsProcessor( root, [] ( CTreeBfsProcessor::Node node ) {
		node->DeleteSelection();
	} );
}

CEquationPresenter::~CEquationPresenter() {}

void CEquationPresenter::InsertSymbol( wchar_t symbol ) 
{
	int symbolWidth = view.GetSymbolWidth( symbol, caret.GetCurEdit()->GetRect().GetHeight() );
	caret.GetCurEdit()->InsertSymbol( symbol, caret.Offset(), symbolWidth );
	++caret.Offset();

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::DeleteSymbol() 
{
	if( caret.Offset() != 0 ) {
		caret.GetCurEdit()->DeleteSymbol( caret.Offset() - 1 );
		--caret.Offset();

		invalidateTree();

		view.Redraw();
	}
}

void CEquationPresenter::OnDraw() 
{
	drawProcessor.Process();
	
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

void CEquationPresenter::setCaretPos( int x, int y, CCaret& curCaret ) 
{
	auto predicate = [=] ( CTreeBfsProcessor::Node node ) -> bool {
		return node->GetRect().IsContain( x, y ) && node->GetType() == TEXT;
	};
	auto hint = [=] ( CTreeBfsProcessor::Node child ) -> bool {
		return child->GetRect().IsContain( x, y );
	};

	CTreeBfsProcessor processor( root );
	auto firstCandidate = processor.Find( predicate, hint );
	if( firstCandidate == nullptr ) {
		return;
	}
	if( curCaret.GetCurEdit() != firstCandidate ) {
		curCaret.SetCurEdit( firstCandidate );
	}

	std::pair<int, int> newCaretPos = findCaretPos( curCaret.GetCurEdit( ), x );
	curCaret.Offset() = newCaretPos.second;
}

void CEquationPresenter::SetCaret( int x, int y ) 
{
	setCaretPos( x, y, caret );
	deleteSelectionProcessor.Process();
	view.Redraw();
}

void CEquationPresenter::SetSelection( int x, int y ) {
	CCaret selectionCaret;
	setCaretPos( x, y, selectionCaret );
	while( selectionCaret.GetCurEdit() != nullptr && 
		( selectionCaret.GetCurEdit() != caret.GetCurEdit( ) ||
		selectionCaret.GetCurEdit() == caret.GetCurEdit() && selectionCaret.Offset() != caret.Offset() ) ) 
	{
		// Если правее или ниже - двигаемся вправо, иначе - влево
		if( selectionCaret.GetPointX() >= caret.GetPointX() || 
			selectionCaret.GetPointY() < caret.GetPointY() && caret.GetCurEdit()->HasInverseDirection() ) 
		{
			caret.GetCurEdit()->MoveCaretRight( caret.GetCurEdit().get(), caret, true );
		} else {
			caret.GetCurEdit()->MoveCaretLeft( caret.GetCurEdit().get(), caret, true );
		}
	}
	view.Redraw();
}

void CEquationPresenter::MoveCaretLeft() {
	caret.GetCurEdit()->MoveCaretLeft( caret.GetCurEdit().get(), caret );
	view.Redraw();
}

void CEquationPresenter::MoveCaretRight() {
	caret.GetCurEdit()->MoveCaretRight( caret.GetCurEdit().get(), caret );
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

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent ) 
{
	std::shared_ptr<CDegrControlModel> degrModel( new CDegrControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	degrModel->InitializeChildren();
	parent->AddChildAfter( degrModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit( )->SliceEditControl( caret.Offset( ) );
	parent->AddChildAfter( newEditControl, degrModel );

	invalidateTree();
	
	view.Redraw();
}

void CEquationPresenter::addSubscript(std::shared_ptr<CExprControlModel> parent)
{
	std::shared_ptr<CSubscriptControlModel> subscriptModel( new CSubscriptControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	subscriptModel->InitializeChildren();
	parent->AddChildAfter( subscriptModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, subscriptModel );

	invalidateTree();
	
	view.Redraw();
}

void CEquationPresenter::addRadical(std::shared_ptr<CExprControlModel> parent)
{

	std::shared_ptr<CRadicalControlModel> radicalModel( new CRadicalControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	radicalModel->InitializeChildren();
	parent->AddChildAfter( radicalModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, radicalModel );

	invalidateTree();
	
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

	MoveCaretRight();
}

void CEquationPresenter::invalidateTree( )
{
	invalidateBranch( root );
}

void CEquationPresenter::invalidateBranch( std::shared_ptr<IBaseExprModel> startingNode ) {
	resizeProcessor.SetStartingNode( startingNode );
	placeProcessor.SetStartingNode( startingNode );
	highlightingProcessor.SetStartingNode( startingNode );

	resizeProcessor.Process();
	placeProcessor.Process();
	highlightingProcessor.Process( );
}