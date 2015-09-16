#include "EquationPresenter.h"

CEquationPresenter::CEquationPresenter() 
{
	RECT rect;
	rect.bottom = 0;
	rect.left = 0;
	rect.top = 20;
	rect.right = 30;

	root = new CExprControlModel();
	root->SetRect( rect );
	views[root] = new CExprControlView();
	views[root]->SetRect( rect );
}

IBaseExprModel* CEquationPresenter::getControlView( HWND hwnd )
{
	std::queue< IBaseExprModel* > queue;
	queue.push(root);

	while( !queue.empty() ) {
		IBaseExprModel* curNode = queue.front( );
		queue.pop();
		if( views[curNode]->GetHWND() == hwnd ) {
			return curNode;
		}
		for( IBaseExprModel* model : curNode->GetChilds() ) {
			queue.push( model );
		}
	}
	return nullptr;
}

void CEquationPresenter::AddControlView( IControlView* newView, HWND currViewHwnd )
{
	IBaseExprModel* focusedModel = getControlView( currViewHwnd );

	if( focusedModel != nullptr ) {
		IBaseExprModel* newModel;

		switch( newView->GetType() ) {
		case EDIT_CONTROL:
			newModel = new CEditControlModel();
		default:
			break;
		}

		IBaseExprModel* parent = focusedModel->GetParent( );
		if( parent == nullptr ) {
			parent = root;
		}
		parent->AddChild( newModel );
		newModel->SetParent( parent );
		RECT rect = focusedModel->GetRect();
		rect.bottom += 20;
		rect.left += 20;
		rect.right += 20;
		rect.top += 20;
		newModel->SetRect( rect );
		newView->SetRect( rect );
		views[newModel] = newView;
	}
}