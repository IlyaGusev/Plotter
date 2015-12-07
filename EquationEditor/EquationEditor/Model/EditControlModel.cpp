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
    if ( i < symbolsWidths.size() )
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

int CEditControlModel::getPrecedence(std::wstring operation) {
	if (operation == L">/sulp<>ylppa<" || operation == L">/sunim<>ylppa<")
		return 2;

	if (operation == L">/semit<>ylppa<" || operation == L">/edivid<>ylppa<")
		return 3;

	if (operation == L">/qe<>ylppa<")
		return 4;
}

std::wstring CEditControlModel::Wrap(std::wstring &text, bool isNumber) {
	if (text == L"")
		return L"";
	/*if (isNumber)
		return L">nc/<" + text + L">nc<";
	else
		return L">ic/<" + text + L">ic<";*/
	if (isNumber)
		return L"<cn>" + text + L"</cn>";
	else
		return L"<ci>" + text + L"</ci>";
}

std::wstring CEditControlModel::ParseText() {
	std::wstring result = L""; 
	std::stack <std::wstring> operationStack;

	////
	bool isMult = false, doMult = false;
	std::stack <std::wstring> numberStack;

	std::wstring currentWord = L"";
	bool isNumber = true;
	int textLength = params.text.length() - 1;
	for (int i = 0; i <= textLength; i++) {
		if ((params.text[i] <= L'9') && (params.text[i] >= L'0') || (params.text[i] <= L'z') && (params.text[i] >= L'a')) {
			currentWord += params.text[i];
			isNumber = isNumber && (params.text[i] <= L'9') && (params.text[i] >= L'0');
			if( i == textLength )
				numberStack.push(Wrap(currentWord, isNumber));
		}
		else
		{
			numberStack.push(Wrap(currentWord, isNumber));
			currentWord = L"";
			isNumber = true;
			if (isMult)
				doMult = true;
		}

		if (doMult) {
			std::wstring tmp;
			tmp = operationStack.top() + numberStack.top();
			numberStack.pop();
			operationStack.pop();
			tmp += numberStack.top() + L"</apply>";
			numberStack.pop();
			numberStack.push(tmp);
			doMult = false;
		}

		if (operations.find(params.text[i]) != operations.end()) {
			std::wstring tmp;
			switch (params.text[i]) {
			case L'+':
				tmp = L"<apply><plus/>";
				operationStack.push(tmp);
				isMult = false;
				break;
			case L'-':
				tmp = L"<apply><minus/>";
				operationStack.push(tmp);
				isMult = false;
				break;
			case L'*':
				tmp = L"<apply><times/>";
				operationStack.push(tmp);
				isMult = true;
				break;
			case L'/':
				tmp = L"<apply><divide/>";
				operationStack.push(tmp);
				isMult = true;
				break;
			case L'=':
				result += L"<apply><eq/>";
				while (!operationStack.empty()) {
					result += L"<apply>" + operationStack.top() + numberStack.top();
					operationStack.pop();
					numberStack.pop();
					result += numberStack.top() + L"</apply>";
				}
				result += L"</apply>";
				isMult = false;
				break;
			default:
				break;
			}
		}
	};
	result += operationStack.top() + numberStack.top();
	operationStack.pop();
	numberStack.pop();
	result += numberStack.top() + L"</apply>";
	numberStack.pop();
	while (!numberStack.empty()) {
		result = operationStack.top() + result;
		operationStack.pop();
		result += numberStack.top() + L"</apply>";
		numberStack.pop();
	}
	result = std::wstring(result);
	/*
	std::wstring currentWord = L"";
	bool isNumber = true;
	int i = params.text.length() - 1;
	while (i >= 0) {
		if ((params.text[i] <= L'9') && (params.text[i] >= L'0') || (params.text[i] <= L'z') && (params.text[i] >= L'a')) {
			currentWord += params.text[i];
			isNumber = isNumber && (params.text[i] <= L'9') && (params.text[i] >= L'0');
		}
		else
		{
			result += Wrap(currentWord, isNumber);
			currentWord = L"";
			isNumber = true;
		}

		//это очень странно, но нужно для того, чтобы потом перевернуть строку в конце
		if (operations.find(params.text[i]) != operations.end()) {
			std::wstring tmp;
			switch (params.text[i]) {
			case L'+':
				tmp = L">/sulp<>ylppa<";
				break;
			case L'-':
				tmp = L">/sunim<>ylppa<";
				break;
			case L'*':
				tmp = L">/semit<>ylppa<"; //SORRY IM REALLY SORRY
				break;
			case L'/':
				tmp = L">/edivid<>ylppa<";
				break;
			case L'=':
				tmp = L">/qe<>ylppa<";
				break;
			default:
				break;
			}

			while (!operationStack.empty() && (getPrecedence(operationStack.top()) > getPrecedence(tmp)))
			{
				result += operationStack.top();
				operationStack.pop();
				result = L">ylppa/<" + result;
			}
			operationStack.push(tmp);
		}
		i--;
	};

	if (currentWord != L"")
		result += Wrap(currentWord, isNumber);

	while (!operationStack.empty())
	{
		result += operationStack.top();
		operationStack.pop();
		result = L">ylppa/<" + result;
	}

	//reverse the string
	result = std::wstring(result.rbegin(), result.rend());
	*/
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