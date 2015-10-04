#include "Model/EditControlModel.h"

CEditControlModel::CEditControlModel( CRect rect, const std::weak_ptr<IBaseExprModel> parent ) 
{
	this->parent = parent;
	this->rect = rect;
}

void CEditControlModel::Resize( )
{
	if( params.text.length() == 0 ) {
		rect.Right() = rect.Left() + 10;
	}
	else {
		auto sum = 0;
		for( auto width : symbolsWidths ) {
			sum += width;
		}
		rect.Right() = rect.Left() + sum;
	}
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
	rect.Right() += symbolWidth;
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
	std::shared_ptr<CEditControlModel> newEditControl( new CEditControlModel( rect, parent.lock() ) );

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
