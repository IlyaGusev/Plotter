#include "Model/ParenthesesControlModel.h"

#include <string>

CParenthesesControlModel::CParenthesesControlModel( CRect rect, std::weak_ptr<IBaseExprModel> parent ) :
	IBracketsModel( rect, parent )
{
	depth = parent.lock()->GetDepth() + 1;
}

std::wstring CParenthesesControlModel::Serialize() {
	if (!content->IsEmpty()) {
		return L"<apply>" + content->Serialize() + L"</apply>";
	}
	return L"";
}

ViewType CParenthesesControlModel::GetType( ) const
{
	return PARENTHESES;
}

void CParenthesesControlModel::updatePolygons()
{
	params.polygon.clear();
	// ����� ������
	params.polygon.push_back( CLine( rect.Left(), rect.Top() + 3, rect.Left(), rect.Bottom() - 3 ) ); // ��������
	params.polygon.push_back( CLine( rect.Left(), rect.Top() + 3, rect.Left() + 3, rect.Top() ) ); // ����
	params.polygon.push_back( CLine( rect.Left(), rect.Bottom() - 3, rect.Left() + 3, rect.Bottom() ) ); // ���
	// ������ ������
	params.polygon.push_back( CLine( rect.Right(), rect.Top() + 3, rect.Right(), rect.Bottom() - 3 ) ); // ��������
	params.polygon.push_back( CLine( rect.Right(), rect.Top() + 3, rect.Right() - 3, rect.Top() ) ); // ����
	params.polygon.push_back( CLine( rect.Right(), rect.Bottom() - 3, rect.Right() - 3, rect.Bottom() ) ); // ���
}

std::shared_ptr<IBaseExprModel> CParenthesesControlModel::CopySelected() const
{
	// ���� �� ������ ������ �������� - ���������� ��� ��������� � ������,
	// ����� ���������� ������ ���������� �����
	if( content->IsSelected() ) {
		std::shared_ptr<CParenthesesControlModel> parModel( new CParenthesesControlModel( rect, parent ) );
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
