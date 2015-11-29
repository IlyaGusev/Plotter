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
	// ����� ������

	int height = rect.GetHeight();

	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + 3, rect.Left() + 2, rect.Top() + height / 2.0 - 2 ) ); // ������� ������������ �����
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + height / 2.0 + 2, rect.Left() + 2, rect.Bottom() - 3 ) ); // ������ ������������ �����

	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + height / 2.0 - 2, rect.Left(), rect.Top() + height / 2.0 ) ); // ������� ��������� "������"
	params.polygon.push_back( CLine( rect.Left(), rect.Top() + height / 2.0, rect.Left() + 2, rect.Top() + height / 2.0 + 2 ) ); // ������ ��������� "������"

	params.polygon.push_back( CLine( rect.Left() + 2, rect.Top() + 3, rect.Left() + 5, rect.Top() ) ); // ������� ����������
	params.polygon.push_back( CLine( rect.Left() + 2, rect.Bottom() - 3, rect.Left() + 5, rect.Bottom() ) ); // ������ ����������

	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + 3, rect.Right() - 2, rect.Top() + height / 2.0 - 2 ) ); // ������� ������������ �����
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + height / 2.0 + 2, rect.Right() - 2, rect.Bottom() - 3 ) ); // ������ ������������ �����

	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + height / 2.0 - 2, rect.Right(), rect.Top() + height / 2.0 ) ); // ������� ��������� "������"
	params.polygon.push_back( CLine( rect.Right(), rect.Top() + height / 2.0, rect.Right() - 2, rect.Top() + height / 2.0 + 2 ) ); // ������ ��������� "������"

	params.polygon.push_back( CLine( rect.Right() - 2, rect.Top() + 3, rect.Right() - 5, rect.Top() ) ); // ������� ����������
	params.polygon.push_back( CLine( rect.Right() - 2, rect.Bottom() - 3, rect.Right() - 5, rect.Bottom() ) ); // ������ ����������
}

std::shared_ptr<IBaseExprModel> CBracesControlModel::CopySelected() const
{
	// ���� �� ������ ������ �������� - ���������� ��� ��������� � ������,
	// ����� ���������� ������ ���������� �����
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

