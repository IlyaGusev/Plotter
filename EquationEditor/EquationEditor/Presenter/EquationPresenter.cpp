#include "Presenter/EquationPresenter.h"

#include "Model/FracControlModel.h"
#include "Model/DegrControlModel.h"
#include "Model/SubscriptControlModel.h"
#include "Model/RadicalControlModel.h"
#include "Model/ParenthesesControlModel.h"
#include "Model/SquareBracketsControlModel.h"
#include "Model/BracesControlModel.h"
#include "Model/ProductControlModel.h"
#include "Model/SumControlModel.h"
#include "Model/SystemControlModel.h"

CEquationPresenter::CEquationPresenter( IEditorView& newView ) :
	view( newView ),
	isInSelectionMode( false )
{
	CRect rect( 20, 30, 30, 50 );
	deltaY = 0;

	root = std::make_shared<CExprControlModel>( rect, std::weak_ptr<IBaseExprModel>() );
	root->InitializeChildren();
	caret.SetCurEdit( root->GetChildren().front() );

	// initialize tree invalidate processors
	highlightingProcessor = CTreeBfsProcessor( root, []( CTreeBfsProcessor::Node node ) {
		// Если у exprControl единственный ребенок - это текст. Если текст пустой - соответствующий editControl надо подсветить
		if( node->GetType() == EXPR ) {
			if( node->IsEmpty() ) {
				node->GetChildren().front()->SetHighlighting( true );
			} else {
				for( auto child : node->GetChildren() ) {
					child->SetHighlighting( false );
				}
			}
		}
	} );

	resizeProcessor = CTreeDfsProcessor( root );
	resizeProcessor.SetExitProcessFunc( []( CTreeDfsProcessor::Node node ) {
		node->Resize();
	} );

	placeProcessor = CTreeBfsProcessor( root, []( CTreeBfsProcessor::Node node ) {
		node->PlaceChildren();
	} );

	// initialize draw processor
	drawProcessor = CTreeBfsProcessor( root, [=]( CTreeBfsProcessor::Node node ) {
		if( node->IsSelected() ) {
			CRect rect = node->GetRect();
			rect.MoveBy( 0, -deltaY );
			view.DrawSelectedRect( rect );
		}
		if( !node->GetLines().empty() ) {
			std::list<CLine> lines;
			for( CLine line : node->GetLines() ) {
				line.MoveBy( 0, -deltaY );
				lines.push_back( line );
			}
			view.DrawPolygon( lines, node->IsSelected() );
		}
		for( std::pair<std::wstring, CRect> selectedText : node->GetSelectedText() ) {
			selectedText.second.MoveBy( 0, -deltaY );
			view.DrawString( selectedText.first, selectedText.second, true );
		}
		for( std::pair<std::wstring, CRect> selectedText : node->GetUnselectedText() ) {
			selectedText.second.MoveBy( 0, -deltaY );
			view.DrawString( selectedText.first, selectedText.second, false );
		}
		if( node->IsHighlighted() ) {
			CRect rect = node->GetRect();
			rect.MoveBy( 0, -deltaY );
			view.DrawHighlightedRect( rect, node->IsSelected() );
		}
	} );

	deleteSelectionProcessor = CTreeBfsProcessor( root, []( CTreeBfsProcessor::Node node ) {
		node->DeleteSelection();
	} );

	updateSelectionProcessor = CTreeBfsProcessor( root, []( CTreeBfsProcessor::Node node ) {
		node->UpdateSelection();
	} );
}

void CEquationPresenter::SetDelta( int delta )
{
	deltaY = delta;
}

void CEquationPresenter::deleteSelectedParts()
{
	// Ставим каретку левее
	caret = (isRightDirection( caret.GetCurEdit().get(), beginSelectionCaret.GetCurEdit().get(),
		caret.Offset(), beginSelectionCaret.Offset() ))
		? caret : beginSelectionCaret;
	// Уходим из зоны удаления
	if( root->GetChildren().front()->IsSelected() ) {
		root->AddChildBefore( std::make_shared<CEditControlModel>( root->GetChildren().front()->GetRect(), root ),
			root->GetChildren().front() );
	}
	while( caret.GetCurEdit()->IsSelected() ) {
		caret.GetCurEdit()->MoveCaretLeft( caret.GetCurEdit().get(), caret );
	}

	root->DeleteSelectedPart();	// processor
}

void CEquationPresenter::InsertSymbol( wchar_t symbol )
{
	if( isInSelectionMode ) {
		deleteSelectedParts();
		isInSelectionMode = false;
		deleteSelectionProcessor.Process();
	}
	int symbolWidth = view.GetSymbolWidth( symbol, caret.GetCurEdit()->GetRect().GetHeight() );
	caret.GetCurEdit()->InsertSymbol( symbol, caret.Offset(), symbolWidth );
	++caret.Offset();

	invalidateTree();
	view.Redraw();
}

void CEquationPresenter::DeleteSymbol( bool withCtrl )
{
	// если курсор стоит на пустой строчке в системе уравнений, надо её убить 
	std::shared_ptr<IBaseExprModel> next = nullptr;
	auto parentSystem = NearestSystem( caret.GetCurEdit() );
	if( parentSystem != nullptr ) {
		int line = parentSystem->FindLineNum( caret.GetCurEdit() );
		if( parentSystem->CanRemoveChild( line ) ) {
			MoveCaretLeft();
		}

		next = parentSystem->TryRemoveChild( line );
		if( next != nullptr ) {
			invalidateTree();
			view.Redraw();
			return;
		}
	}
	// иначе просто обычная строчка
	if( isInSelectionMode ) {
		deleteSelectedParts();
		isInSelectionMode = false;
		deleteSelectionProcessor.Process();
	} else if( caret.Offset() != 0 ) {
		caret.GetCurEdit()->DeleteSymbol( caret.Offset() - 1 );
		--caret.Offset();
		if( withCtrl ) {
			while( caret.Offset() != 0 ) {
				caret.GetCurEdit()->DeleteSymbol( caret.Offset() - 1 );
				--caret.Offset();
			};
		}
	} else if( !withCtrl ) {
		do {
			SetSelection( caret.GetPointX(), caret.GetPointY() );
			deleteSelectedParts();
			isInSelectionMode = false;
			deleteSelectionProcessor.Process();
			//MoveCaretLeft();
		} while( caret.GetCurEdit() != root->GetChildren().front() && caret.Offset() == 0 );
	}

	invalidateTree();
	view.Redraw();
}

std::wstring CEquationPresenter::Serialize()
{
    return L"" + root->Serialize();
	return L"<math>"+root->Serialize()+ L"</math>";
}

void CEquationPresenter::DeleteNextSymbol( bool withCtrl )
{
	if( isInSelectionMode ) {
		deleteSelectedParts();
		isInSelectionMode = false;
		deleteSelectionProcessor.Process();
	} else if( caret.Offset() != caret.GetCurEdit()->GetSymbolsWidths().size() ) {
		caret.GetCurEdit()->DeleteSymbol( caret.Offset() );
		if( withCtrl ) {
			while( caret.Offset() != caret.GetCurEdit()->GetSymbolsWidths().size() ) {
				caret.GetCurEdit()->DeleteSymbol( caret.Offset() );
			}
		}
	} else if( !withCtrl ) {
		do {
			SetSelection( caret.GetPointX(), caret.GetPointY() );
			deleteSelectedParts();
			isInSelectionMode = false;
			deleteSelectionProcessor.Process();
			//MoveCaretRight();
		} while( caret.GetCurEdit() != root->GetChildren().back() && caret.GetCurEdit()->GetSymbolsWidths().size() == 0 );
	}

	invalidateTree();
	view.Redraw();
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
	return std::make_pair( length, offset );
}

void CEquationPresenter::setCaretPos( int x, int y, CCaret& curCaret )
{
	auto selectX = [=]( CTreeBfsProcessor::Node node ) -> bool
	{
		auto rect = node->GetRect();
		return node->GetType() == TEXT && (rect.Top() <= y) && (y <= rect.Bottom());
	};
	auto selectY = [=]( CTreeBfsProcessor::Node node ) -> bool
	{
		auto rect = node->GetRect();
		return node->GetType() == TEXT && (rect.Left() <= x) && (x <= rect.Right());
	};
	auto selectAll = [=]( CTreeBfsProcessor::Node node )->bool
	{
		return node->GetType() == TEXT;
	};

	CTreeBfsProcessor processor( root );
	CTreeBfsProcessor::Node chosenCandidate = nullptr;
	auto candidates = processor.FindAll( selectX );
	if( candidates.size() == 0 ) {
		candidates = processor.FindAll( selectY );
		if( candidates.size() == 0 ) {
			candidates = processor.FindAll( selectAll );
			if( candidates.size() == 0 ) {
				throw std::runtime_error( "Tree has no edit controls, something went wrong." );
			}
#undef max  // передаю пламенный привет ребятам из microsoft, которые перегрузили max в minwindef.h
			int minDistance = std::numeric_limits<int>::max();
			for( auto candidate : candidates ) {
				if( candidate->GetRect().IsContain( x, y ) ) {
					chosenCandidate = candidate;
					break;
				}
				int distance =
					MIN( std::abs( candidate->GetRect().Top() - y ), std::abs( candidate->GetRect().Bottom() - y ) ) +
					MIN( std::abs( candidate->GetRect().Right() - x ), std::abs( candidate->GetRect().Left() - x ) );
				if( distance < minDistance ) {
					minDistance = distance;
					chosenCandidate = candidate;
				}
			}
		} else {
			int minDistance = std::numeric_limits<int>::max();
			for( auto candidate : candidates ) {
				auto rect = candidate->GetRect();
				if( rect.Top() <= y && rect.Bottom() >= y ) {
					chosenCandidate = candidate;
					break;
				}
				int distance = MIN( std::abs( rect.Top() - y ), std::abs( rect.Bottom() - y ) );
				if( distance < minDistance ) {
					minDistance = distance;
					chosenCandidate = candidate;
				}
			}
		}
	} else {
		int minDistance = std::numeric_limits<int>::max();
		for( auto candidate : candidates ) {
			auto rect = candidate->GetRect();
			if( rect.Right() >= x && rect.Left() <= x ) {
				chosenCandidate = candidate;
				break;
			}
			int distance = MIN( std::abs( rect.Right() - x ), std::abs( rect.Left() - x ) );
			if( distance < minDistance ) {
				minDistance = distance;
				chosenCandidate = candidate;
			}
		}
	}

	if( curCaret.GetCurEdit() != chosenCandidate ) {
		curCaret.SetCurEdit( chosenCandidate );
	}

	std::pair<int, int> newCaretPos = findCaretPos( curCaret.GetCurEdit(), x );
	curCaret.Offset() = newCaretPos.second;
}

void CEquationPresenter::SetCaret( int x, int y )
{
	isInSelectionMode = false;
	deleteSelectionProcessor.Process();
	setCaretPos( x, y, caret );
	view.Redraw();
}

bool CEquationPresenter::isRightDirection( const IBaseExprModel* model1, const IBaseExprModel* model2, int offset1, int offset2 )
{
	if( model1 == model2 ) {
		return offset1 < offset2;
	}
	while( model1->GetDepth() > model2->GetDepth() ) {
		model1 = model1->GetParent().lock().get();
	}
	while( model2->GetDepth() > model1->GetDepth() ) {
		model2 = model2->GetParent().lock().get();
	}
	while( model1->GetParent().lock() != model2->GetParent().lock() ) {
		model1 = model1->GetParent().lock().get();
		model2 = model2->GetParent().lock().get();
	}
	int first, second;
	int i = 0;
	auto children = model1->GetParent().lock()->GetChildren();
	for( auto it = children.begin(); it != children.end(); ++it, ++i ) {
		if( (*it).get() == model1 ) {
			first = i;
		}
		if( (*it).get() == model2 ) {
			second = i;
		}
	}
	return first < second;
}

void CEquationPresenter::SetSelection( int x, int y )
{
	if( !isInSelectionMode ) {
		beginSelectionCaret = caret;
		isInSelectionMode = true;
	}

	CCaret selectionCaret;
	setCaretPos( x, y, selectionCaret );
	while( selectionCaret.GetCurEdit() != nullptr && selectionCaret != caret ) {
		if( isRightDirection( caret.GetCurEdit().get(), selectionCaret.GetCurEdit().get(), caret.Offset(), selectionCaret.Offset() ) ) {
			caret.GetCurEdit()->MoveCaretRight( caret.GetCurEdit().get(), caret, true );
		} else {
			caret.GetCurEdit()->MoveCaretLeft( caret.GetCurEdit().get(), caret, true );
		}
	}

	updateSelectionProcessor.Process();
	view.Redraw();
}

void CEquationPresenter::MoveCaretLeft()
{
	caret.GetCurEdit()->MoveCaretLeft( caret.GetCurEdit().get(), caret );
	view.Redraw();
}

void CEquationPresenter::MoveCaretRight()
{
	caret.GetCurEdit()->MoveCaretRight( caret.GetCurEdit().get(), caret );
	view.Redraw();
}

std::shared_ptr<CSystemControlModel> CEquationPresenter::NearestSystem( std::shared_ptr<CEditControlModel> edit ) // ближайший слева объект "системы уравнений"
{
	std::weak_ptr<IBaseExprModel> parentSystem = edit;
	while( parentSystem.lock().get() != nullptr && parentSystem.lock().get()->GetType() != SYSTEM ) {
		parentSystem = parentSystem.lock().get()->GetParent();
	}

	return std::dynamic_pointer_cast< CSystemControlModel >(parentSystem.lock());
}

void CEquationPresenter::OnEnter()
{
	std::shared_ptr<CSystemControlModel> parentSystem = NearestSystem( caret.GetCurEdit() );

	if( parentSystem != nullptr ) { // if caret is in equation system
		int lineNum = parentSystem->FindLineNum( caret.GetCurEdit() ) + 1; // надо поместить новый контрол на следующую строку
		parentSystem->AddChild( lineNum, nullptr ); // вторым аргументом мог бы быть IBaseExprModel для вставки, но нет
		invalidateTree();
		view.Redraw();

		std::list<std::shared_ptr<IBaseExprModel>> children = parentSystem->GetChildren();
		auto it = children.begin();
		std::advance( it, lineNum );
		caret.SetCurEdit( it->get()->GetChildren().front() );
		caret.SetOffset( 0 );

		invalidateTree();
		view.Redraw();
	} else {
		// if we're not in system -- keep calm and wait for realization
	}
}

void CEquationPresenter::addFrac( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	// Создаем новые модели для дроби
	std::shared_ptr<CFracControlModel> fracModel( new CFracControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	fracModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( fracModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, fracModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addSum( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	// Создаем новые модели для суммы
	std::shared_ptr<CSumControlModel> sumModel( new CSumControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	sumModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( sumModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, sumModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addProduct( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	// Создаем новые модели для произведения
	std::shared_ptr<CProductControlModel> productModel( new CProductControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	productModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( productModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, productModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addDegr( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CDegrControlModel> degrModel( new CDegrControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	degrModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( degrModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, degrModel );


	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addSubscript( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CSubscriptControlModel> subscriptModel( new CSubscriptControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	subscriptModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( subscriptModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, subscriptModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addParentheses( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CParenthesesControlModel> parenthesesModel( new CParenthesesControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	parenthesesModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( parenthesesModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, parenthesesModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addBraces( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CBracesControlModel> bracesModel( new CBracesControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	bracesModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( bracesModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, bracesModel );

	invalidateTree();

	view.Redraw();
}


void CEquationPresenter::addSquareBrackets( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CSquareBracketsControlModel> squareBracketsModel( new CSquareBracketsControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	squareBracketsModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( squareBracketsModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, squareBracketsModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addRadical( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CRadicalControlModel> radicalModel( new CRadicalControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	radicalModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( radicalModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, radicalModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::addSystem( std::shared_ptr<CExprControlModel> parent, std::shared_ptr<CExprControlModel> selectedChild )
{
	std::shared_ptr<CSystemControlModel> systemModel( new CSystemControlModel( caret.GetCurEdit()->GetRect(), parent ) );
	systemModel->InitializeChildren( selectedChild );
	parent->AddChildAfter( systemModel, caret.GetCurEdit() );

	std::shared_ptr<CEditControlModel> newEditControl = caret.GetCurEdit()->SliceEditControl( caret.Offset() );
	parent->AddChildAfter( newEditControl, systemModel );

	invalidateTree();

	view.Redraw();
}

void CEquationPresenter::AddControlView( ViewType viewType )
{
	std::shared_ptr<CExprControlModel> selectedChild;
	if( isInSelectionMode ) {
		selectedChild = std::dynamic_pointer_cast< CExprControlModel >(root->CopySelected());
		deleteSelectedParts();
		isInSelectionMode = false;
		deleteSelectionProcessor.Process();
	}
	// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
	// Родитель должен иметь тип CExprControlModel
	std::shared_ptr<CExprControlModel> parent( std::dynamic_pointer_cast< CExprControlModel >(caret.GetCurEdit()->GetParent().lock()) );
	if( parent == nullptr ) {
		parent = root;
	}

	// Создаем новую вьюшку с выбранным типом
	switch( viewType ) {
		case FRAC:
			addFrac( parent, selectedChild );
			break;
		case DEGR:
			addDegr( parent, selectedChild );
			break;
		case SUBSCRIPT:
			addSubscript( parent, selectedChild );
			break;
		case RADICAL:
			addRadical( parent, selectedChild );
			break;
		case PARENTHESES:
			addParentheses( parent, selectedChild );
			break;
		case SQUAREBRACKETS:
			addSquareBrackets( parent, selectedChild );
			break;
		case BRACES:
			addBraces( parent, selectedChild );
			break;
		case SUM:
			addSum( parent, selectedChild );
			break;
		case PRODUCT:
			addProduct( parent, selectedChild );
			break;
		case SYSTEM:
			addSystem( parent, selectedChild );
			break;
		default:
			break;
	}

	MoveCaretRight();
}

void CEquationPresenter::invalidateTree()
{
	invalidateBranch( root );
}

void CEquationPresenter::invalidateBranch( std::shared_ptr<IBaseExprModel> startingNode )
{
	highlightingProcessor.SetStartingNode( startingNode );
	resizeProcessor.SetStartingNode( startingNode );
	placeProcessor.SetStartingNode( startingNode );

	highlightingProcessor.Process();
	resizeProcessor.Process();
	placeProcessor.Process();
}