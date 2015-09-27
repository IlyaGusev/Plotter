#include "Model/EditControlModel.h"

CEditControlModel::CEditControlModel() {
	parent = nullptr;
}

void CEditControlModel::Resize( )
{
}

void CEditControlModel::PermutateChildren( )
{
}

int CEditControlModel::GetMiddle( ) const
{
	return (rect.Bottom() - rect.Top()) / 2;
}

std::list< std::shared_ptr<IBaseExprModel> > CEditControlModel::GetChildren() const {
	return children;
}

void CEditControlModel::InsertSymbol( wchar_t symbol, int offset, int symbolWidth ) {
	params.text.insert( offset, 1, symbol );
	rect.Right() += symbolWidth;
	symbolsWidths.push_back( symbolWidth );
}

int CEditControlModel::DeleteSymbol( int offset ) {
	params.text.erase( offset, 1 );
	int symbolsWidth = symbolsWidths[offset];
	rect.Right() -= symbolsWidth;
	symbolsWidths.erase( symbolsWidths.begin() + offset );
	return symbolsWidth;
}

std::shared_ptr<CEditControlModel> CEditControlModel::SliceEditControl( int offset ) {
	std::shared_ptr<CEditControlModel> newEditControl( new CEditControlModel() );

	// Вставляем всё, начиная с offset, в новый edit control
	int newEditControlWidth = 0;
	for( size_t i = offset; i < symbolsWidths.size(); ++i ) {
		newEditControlWidth += symbolsWidths[i];
		newEditControl->symbolsWidths.push_back( symbolsWidths[i] );
		newEditControl->params.text.push_back( params.text[i] );
	}
	newEditControl->parent = parent;
	newEditControl->rect = rect;
	newEditControl->rect.Left() = rect.Right() - newEditControlWidth;

	// Удаляем всё до offset из старого
	rect.Right() = newEditControl->rect.Left();
	params.text.resize( offset );
	symbolsWidths.resize( offset );
	return newEditControl;
}

std::vector<int> CEditControlModel::GetSymbolsWidths() {
	return symbolsWidths;
}

ViewType CEditControlModel::GetType( ) const {
	return TEXT;
}

