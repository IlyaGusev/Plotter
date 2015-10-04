#include "Model/EditControlModel.h"

CEditControlModel::CEditControlModel( CRect rect, const std::weak_ptr<IBaseExprModel> parent, bool isHightlighted ) 
{
	this->parent = parent;
	this->rect = rect;
	this->params.isHightlighted = isHightlighted;
}

void CEditControlModel::Resize( )
{
}

void CEditControlModel::PlaceChildren( )
{
}

int CEditControlModel::GetMiddle( ) const
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
	if( params.isHightlighted ) {
		params.isHightlighted = false;
		rect.Right() = rect.Left() + symbolWidth;
	} else {
		rect.Right() += symbolWidth;
	}
	symbolsWidths.push_back( symbolWidth );
}

int CEditControlModel::DeleteSymbol( int offset ) 
{
	params.text.erase( offset, 1 );
	int symbolsWidth = symbolsWidths[offset];
	rect.Right() -= symbolsWidth;
	symbolsWidths.erase( symbolsWidths.begin() + offset );
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

void CEditControlModel::GoLeft( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const {
	// Если это не тот эдит, с которого начали движение - останавливаемся на нем в самом конце
	if( from.get() != this ) {
		caret.SetCurEdit( std::const_pointer_cast<IBaseExprModel>( shared_from_this() ) );
		caret.Offset() = symbolsWidths.size();
		return;
	}
	// Если тот - двигаемся вдоль него
	if( caret.Offset() > 0 ) {
		--caret.Offset();
	} else {
		parent.lock()->GoLeft( shared_from_this(), caret );
	}
}

void CEditControlModel::GoRight( std::shared_ptr<const IBaseExprModel> from, CCaret& caret ) const {
	// Если это не тот эдит, с которого начали движение - останавливаемся на нем в самом начале
	if( from.get() != this ) {
		caret.SetCurEdit( std::const_pointer_cast<IBaseExprModel>(shared_from_this()) );
		caret.Offset() = 0;
		return;
	}
	// Если тот - двигаемся вдоль него
	if( caret.Offset() < symbolsWidths.size() ) {
		++caret.Offset();
	} else {
		parent.lock()->GoRight( shared_from_this(), caret );
	}
}
