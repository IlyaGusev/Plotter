#include "SquareBracketsControlModel.h"

CSquareBracketsControlModel::CSquareBracketsControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBracketsModel( rect, parent )
{
	depth = parent.lock()->GetDepth() + 1;
}

std::wstring CSquareBracketsControlModel::Serialize()
{
	if( !content->IsEmpty() ) {
		return L"<apply>" + content->Serialize() + L"</apply>";
	}
	return L"";
}

ViewType CSquareBracketsControlModel::GetType() const
{
	return SQUAREBRACKETS;
}

void CSquareBracketsControlModel::updatePolygons()
{
	params.polygon.clear();
	// ����� ������
	params.polygon.push_back( CLine( rect.Left(), rect.Top(), rect.Left(), rect.Bottom() ) ); // ��������
	params.polygon.push_back( CLine( rect.Left(), rect.Top(), rect.Left() + 5, rect.Top() ) ); // ����
	params.polygon.push_back( CLine( rect.Left(), rect.Bottom(), rect.Left() + 5, rect.Bottom() ) ); // ���
	// ������ ������
	params.polygon.push_back( CLine( rect.Right(), rect.Top(), rect.Right(), rect.Bottom() ) ); // ��������
	params.polygon.push_back( CLine( rect.Right(), rect.Top(), rect.Right() - 5, rect.Top() ) ); // ����
	params.polygon.push_back( CLine( rect.Right(), rect.Bottom(), rect.Right() - 5, rect.Bottom() ) ); // ���
}

std::shared_ptr<IBaseExprModel> CSquareBracketsControlModel::CopySelected() const
{
	// ���� �� ������ ������ �������� - ���������� ��� ��������� � ������,
	// ����� ���������� ������ ���������� �����
	if( content->IsSelected() ) {
		std::shared_ptr<CSquareBracketsControlModel> parModel( new CSquareBracketsControlModel( rect, parent ) );
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

