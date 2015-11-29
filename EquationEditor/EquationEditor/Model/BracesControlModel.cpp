#include "BracesControlModel.h"

CBracesControlModel::CBracesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ):
	IBracketsModel( rect, parent )
{
	depth = parent.lock()->GetDepth() + 1;
}

std::wstring CBracesControlModel::Serialize()
{
	if( !content->IsEmpty() ) {
		return L"<apply>" + content->Serialize() + L"</apply>";
	}
	return L"";
}

ViewType CBracesControlModel::GetType() const
{
	return BRACES;
}

void CBracesControlModel::updatePolygons()
{
	params.polygon.clear();
	// левая скобка

	int height = rect.GetHeight();

	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + 3, rect.Left() + 2, rect.Top() + height / 2.0 - 2 ) ); // верхняя вертикальная палка
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + height / 2.0 + 2, rect.Left() + 2, rect.Bottom() - 3 ) ); // нижняя вертикальная палка

	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + height / 2.0 - 2, rect.Left(), rect.Top() + height / 2.0 ) ); // верхняя диагональ "носика"
	params.polygon.push_back( CLine( rect.Left(), rect.Top() + height / 2.0, rect.Left() + 2, rect.Top() + height / 2.0 + 2 ) ); // нижняя диагональ "носика"

	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + 3, rect.Left() + 5, rect.Top() ) ); // верхнее скругление
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Bottom() - 3, rect.Left() + 5, rect.Bottom() ) ); // нижнее скругление

	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + 3, rect.Right() - 2, rect.Top() + height / 2.0 - 2 ) ); // верхняя вертикальная палка
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + height / 2.0 + 2, rect.Right() - 2, rect.Bottom() - 3 ) ); // нижняя вертикальная палка

	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + height / 2.0 - 2, rect.Right(), rect.Top() + height / 2.0 ) ); // верхняя диагональ "носика"
	params.polygon.push_back( CLine( rect.Right(), rect.Top() + height / 2.0, rect.Right() - 2, rect.Top() + height / 2.0 + 2 ) ); // нижняя диагональ "носика"

	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + 3, rect.Right() - 5, rect.Top() ) ); // верхнее скругление
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Bottom() - 3, rect.Right() - 5, rect.Bottom() ) ); // нижнее скругление
}

std::shared_ptr<IBaseExprModel> CBracesControlModel::CopySelected() const
{
	// Если всё внутри скобок выделено - возвращаем его обернутым в скобки,
	// иначе возвращаем только выделенный кусок
	if( content->IsSelected() ) {
		std::shared_ptr<CBracesControlModel> parModel( new CBracesControlModel( rect, parent ) );
		std::shared_ptr<IBaseExprModel> contentModel = content->CopySelected();
		if( contentModel != 0 && !contentModel->IsEmpty() ) {
			parModel->content = contentModel;
			parModel->content->SetParent( parModel );
			return parModel;
		} else {
			return 0;
		}
	} else {
		return content->CopySelected();
	}
}

