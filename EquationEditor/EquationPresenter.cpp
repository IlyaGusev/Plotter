#include "EquationPresenter.h"

CEquationPresenter::CEquationPresenter() 
{
	RECT rect;
	rect.bottom = 40;
	rect.left = 20;
	rect.top = 20;
	rect.right = 35;

	root = new CExprControlModel();
	root->SetRect( rect );
	views[root] = new CExprControlView();
	views[root]->SetRect( rect );
}

IBaseExprModel* CEquationPresenter::getModel( HWND hwnd )
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

void CEquationPresenter::setRect( IBaseExprModel* model, RECT rect ) {
	model->SetRect( rect );
	if( views[model] != nullptr ) {
		views[model]->SetRect( rect );
	}
}

void CEquationPresenter::addFrac( IControlView* view, CExprControlModel* parent, RECT focusedViewRect ) {
	CFracControlView* fracView = dynamic_cast<CFracControlView*>(view);

	if( fracView != nullptr ) {
		CFracControlModel* fracModel = new CFracControlModel();
		CExprControlModel* firstChild = new CExprControlModel();
		CEditControlModel* firstChildEdit = new CEditControlModel();
		CExprControlModel* secondChild = new CExprControlModel();
		CEditControlModel* secondChildEdit = new CEditControlModel();

		// Выставляем размеры вьюшек
		// Высота дроби - две высоты родителя
		RECT fracRect, firstChildRect, secondChildRect;
		firstChildRect.top = fracRect.top = focusedViewRect.top - (focusedViewRect.bottom - focusedViewRect.top) / 2;
		secondChildRect.top = (focusedViewRect.bottom + focusedViewRect.top) / 2 + 1;
		secondChildRect.bottom = fracRect.bottom = focusedViewRect.bottom + (focusedViewRect.bottom - focusedViewRect.top) / 2;
		firstChildRect.bottom = (focusedViewRect.bottom + focusedViewRect.top) / 2 - 1;
		firstChildRect.left = secondChildRect.left = fracRect.left = focusedViewRect.right;
		firstChildRect.right = secondChildRect.right = fracRect.right = focusedViewRect.right + 15;

		// Прикрепляем вьюшки к моделькам
		views[fracModel] = view;
		views[firstChild] = new CExprControlView();
		views[secondChild] = new CExprControlView();
		views[firstChildEdit] = fracView->GetFirstChild();
		views[secondChildEdit] = fracView->GetSecondChild();

		// Посылаем размеры в модели и вьюшки
		setRect( fracModel, fracRect );
		setRect( firstChildEdit, firstChildRect );
		setRect( secondChildEdit, secondChildRect );

		// Обновляем граф
		fracModel->SetParent( parent );
		parent->AddChild( fracModel );
		fracModel->SetFirstChild( firstChild );
		firstChild->SetParent( fracModel );
		fracModel->SetSecondChild( secondChild );
		secondChild->SetParent( fracModel );
		firstChild->AddChild( firstChildEdit );
		firstChildEdit->SetParent( firstChild );
		secondChild->AddChild( secondChildEdit );
		secondChildEdit->SetParent( secondChild );
	}
}

void CEquationPresenter::AddControlView( IControlView* newView, HWND currViewHwnd )
{
	// Ищем по hwnd окна с фокусом соответствующую вьюшку
	IBaseExprModel* focusedModel = getModel( currViewHwnd );

	if( focusedModel != nullptr ) {
		IBaseExprModel* newModel;

		// Подцепляем новую вьюшку к родителю той вьюшки, на которой находился фокус
		// Родитель должен иметь тип CExprControlModel
		CExprControlModel* parent = dynamic_cast<CExprControlModel*>(focusedModel->GetParent());
		if( parent == nullptr ) {
			parent = root;
		}

		// Создаем новую вьюшку с выбранным типом
		switch( newView->GetType() ) {
		case EDIT_CONTROL:
			newModel = new CEditControlModel();
			break;
		case FRAC_CONTROL:
			addFrac( newView, parent, focusedModel->GetRect() );
			return;
		default:
			break;
		}

		views[newModel] = newView;
		setRect( newModel, focusedModel->GetRect() );
		parent->AddChild( newModel );
		newModel->SetParent( parent );
	}
}

void CEquationPresenter::ExtendControlView( HWND currViewHwnd, int deltaWidth ) {
	IBaseExprModel* model = getModel( currViewHwnd );
	if( model != nullptr ) {
		RECT rect = model->GetRect();
		rect.right += deltaWidth;
		setRect( model, rect );
	}
	// updateGraph(model);
}