#include "Model/EditControlModel.h"

CEditControlModel::CEditControlModel( const CRect& rect, const std::weak_ptr<IBaseExprModel> parent, bool isHightlighted ) :
	IBaseExprModel(rect, parent)
{
	this->params.isHighlighted = isHightlighted;
	Resize();
	depth = parent.lock()->GetDepth() + 1;
}

void CEditControlModel::Resize()
{
	if( params.isHighlighted ) {
		rect.Right() = rect.Left() + MINIMAL_WIDTH;
	}
	else {
		auto sum = 0;
		for( auto width : symbolsWidths ) {
			sum += width;
		}
		rect.Right() = rect.Left() + sum;
	}
}

void CEditControlModel::PlaceChildren()
{
}

int CEditControlModel::GetMiddle() const
{
	return rect.GetHeight() / 2;
}

std::list< std::shared_ptr<IBaseExprModel> > CEditControlModel::GetChildren() const 
{
	return children;
}

void CEditControlModel::InsertSymbol( wchar_t symbol, int offset, int symbolWidth ) 
{
	params.text.insert( offset, 1, symbol );
	if( params.isHighlighted ) {
		params.isHighlighted = false;
		rect.Right() = rect.Left() + symbolWidth;
	} else {
		rect.Right() += symbolWidth;
	}
	symbolsWidths.insert( symbolsWidths.begin() + offset, symbolWidth );
}

int CEditControlModel::DeleteSymbol( int offset ) 
{
	params.text.erase( offset, 1 );
	int symbolsWidth = symbolsWidths[offset];
	symbolsWidths.erase( symbolsWidths.begin() + offset );
	if( symbolsWidths.empty() ) {
		params.isHighlighted = true;
	} else {
		rect.Right() -= symbolsWidth;
	}
	return symbolsWidth;
}

std::shared_ptr<CEditControlModel> CEditControlModel::SliceEditControl( int offset ) 
{
	std::shared_ptr<CEditControlModel> newEditControl( new CEditControlModel( rect, parent.lock(), false ) );

	// Вставляем всё, начиная с offset, в новый edit control
	int newEditControlWidth = 0;
	for( size_t i = offset; i < symbolsWidths.size(); ++i ) {
		newEditControlWidth += symbolsWidths[i];
		newEditControl->symbolsWidths.push_back( symbolsWidths[i] );
		newEditControl->params.text.push_back( params.text[i] );
	}
	newEditControl->parent = parent;
	newEditControl->rect.Left() = rect.Right() - newEditControlWidth;

	// Удаляем всё до offset из старого
	rect.Right() = newEditControl->rect.Left();
	params.text.resize( offset );
	symbolsWidths.resize( offset );
	return newEditControl;
}

std::vector<int> CEditControlModel::GetSymbolsWidths() const 
{
	return symbolsWidths;
}

ViewType CEditControlModel::GetType( ) const 
{
	return TEXT;
}

int CEditControlModel::GetSymbolPointByNumber( int number ) const 
{
	int offset = rect.Left();
	for( int i = 0; i < number; ++i ) {
		offset += symbolsWidths[i];
	}
	return offset;
}

void CEditControlModel::DeleteSelection() 
{
	params.isSelected = false;
	params.selectedPositions.first = params.selectedPositions.second = 0;
}

void CEditControlModel::MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode /*= false */ ) 
{
	// Если это не тот эдит, с которого начали движение - останавливаемся на нем в самом конце
	if( from != this ) {
		caret.SetCurEdit( std::const_pointer_cast<IBaseExprModel>( shared_from_this() ) );
		caret.Offset() = symbolsWidths.size();
		return;
	}
	// Если тот - двигаемся вдоль него
	if( caret.Offset() > 0 ) {
		if( isInSelectionMode ) {
			// Позиции могут совпадать, только если еще не начали выделение
			if( params.selectedPositions.first == params.selectedPositions.second ) {
				params.selectedPositions.first = caret.Offset() - 1;
				params.selectedPositions.second = caret.Offset();
			} else {
				if( params.selectedPositions.first > caret.Offset() - 1 ) {
					--params.selectedPositions.first;
				} else {
					--params.selectedPositions.second;
				}
			}
		}
		--caret.Offset();
	} else {
		parent.lock()->MoveCaretLeft( this, caret, isInSelectionMode );
	}
}

void CEditControlModel::MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode/*=false */ ) 
{
	// Если это не тот эдит, с которого начали движение - останавливаемся на нем в самом начале
	if( from != this ) {
		caret.SetCurEdit( std::const_pointer_cast<IBaseExprModel>(shared_from_this()) );
		caret.Offset() = 0;
		return;
	}
	// Если тот - двигаемся вдоль него
	if( caret.Offset() < symbolsWidths.size() ) {
		if( isInSelectionMode ) {
			// Позиции могут совпадать, только если еще не начали выделение
			if( params.selectedPositions.first == params.selectedPositions.second ) {
				params.selectedPositions.first = caret.Offset();
				params.selectedPositions.second = caret.Offset() + 1;
			} else {
				if( params.selectedPositions.second < caret.Offset() + 1 ) {
					++params.selectedPositions.second;
				} else {
					++params.selectedPositions.first;
				}
			}
		}
		++caret.Offset();
	} else {
		parent.lock()->MoveCaretRight( this, caret, isInSelectionMode );
	}
}

std::wstring CEditControlModel::Wrap(std::wstring &text, bool isNumber) {
	if (text == L"")
		return L"";
	if (isNumber)
		return L"<cn>" + text + L"</cn>";
	else
		return L"<ci>" + text + L"</ci>";
}

std::wstring CEditControlModel::ParseText() {
	std::wstring result = L"";

	std::wstring currentWord = L"";
	int i = 0;
	do {
		if ((params.text[i] <= L'9') && (params.text[i] >= L'0') || (params.text[i] <= L'z') && (params.text[i] >= L'a'))
			currentWord += params.text[i];
		else
		{
			result += Wrap(currentWord, true);
			currentWord = L"";
		}

		if (operations.find(params.text[i]) != operations.end())
			switch (params.text[i]) {
			case L'+':
				result += L"<mo>+</mo>";
				break;
			case L'-':
				result += L"<mo>-</mo>";
				break;
			case L'*':
				result += L"<mo>*</mo>";
				break;
			case L'/':
				result += L"<mo>/</mo>";
				break;
			case L'=':
				result += L"<mo>=</mo>";
				break;
			default:
				break;
			}
		i++;
	} while (i <= params.text.length());

	return result;
}

bool CEditControlModel::IsEmpty() const {
	return params.text.empty();
}

std::wstring CEditControlModel::Serialize() {
	//if (!firstChild->IsEmpty()) {
	//	firstChild->Serialize();
	//}
	//if (!secondChild->IsEmpty()) {
	//	secondChild->Serialize();
	//}

	return ParseText();
}

std::list<std::pair<std::wstring, CRect>> CEditControlModel::GetSelectedText() const 
{
	std::wstring selectedString( params.text.begin() + params.selectedPositions.first, params.text.begin() + params.selectedPositions.second );
	return std::list< std::pair<std::wstring, CRect> > { std::make_pair( selectedString, 
		CRect( GetSymbolPointByNumber( params.selectedPositions.first ),
			   rect.Top(),
			   GetSymbolPointByNumber( params.selectedPositions.second ),
			   rect.Bottom() ) ) };
}

std::list<std::pair<std::wstring, CRect>> CEditControlModel::GetUnselectedText( ) const 
{
	std::wstring firstUnselectedString( params.text.begin(), params.text.begin() + params.selectedPositions.first );
	CRect firstRect( rect.Left(), rect.Top(), GetSymbolPointByNumber(params.selectedPositions.first), rect.Bottom() );
	std::wstring secondUnselectedString( params.text.begin() + params.selectedPositions.second, params.text.end() );
	CRect secondRect( GetSymbolPointByNumber( params.selectedPositions.second ), rect.Top(), rect.Right(), rect.Bottom() );
	return std::list<std::pair<std::wstring, CRect>> { std::make_pair( firstUnselectedString, firstRect ), 
													   std::make_pair( secondUnselectedString, secondRect ) };
}

void CEditControlModel::UpdateSelection()
{
	if( params.selectedPositions.first == 0 && params.selectedPositions.second == symbolsWidths.size() ) {
		params.isSelected = true;
	} else {
		params.isSelected = false;
	}
}

bool CEditControlModel::DeleteSelectedPart() 
{
	if( params.selectedPositions.first == params.selectedPositions.second ) {
		return true;
	}
	params.text.erase( params.text.begin() + params.selectedPositions.first, params.text.begin() + params.selectedPositions.second );
	symbolsWidths.erase( symbolsWidths.begin() + params.selectedPositions.first, symbolsWidths.begin() + params.selectedPositions.second );
	params.selectedPositions.first = params.selectedPositions.second = 0;
	return !IsEmpty();
}

void CEditControlModel::MergeWith( const CEditControlModel& edit ) {
	params.text += edit.params.text;
	for( auto symbol : edit.symbolsWidths ) {
		symbolsWidths.push_back( symbol );
	}
}

std::shared_ptr<IBaseExprModel> CEditControlModel::CopySelected() const
{
	// Создаем новую модельку с 
	std::shared_ptr<CEditControlModel> newEditControl( new CEditControlModel( rect, parent.lock(), false ) );
	newEditControl->params.text = std::wstring( params.text.begin() + params.selectedPositions.first,
		params.text.begin() + params.selectedPositions.second );
	newEditControl->symbolsWidths = std::vector<int>( symbolsWidths.begin() + params.selectedPositions.first,
		symbolsWidths.begin() + params.selectedPositions.second );
	newEditControl->SetHighlighting( params.isHighlighted );

	return newEditControl;
}