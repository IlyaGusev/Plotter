#pragma once
#include <list>
#include <memory>

#include "Model/Utils/RectI.h"

#define MIN(x, y) x < y ? x : y;
#define MAX(x, y) x > y ? x : y;

enum ViewType { TEXT, EXPR, FRAC, DEGR };

struct CLine {
	int startX;
	int startY;
	int endX;
	int endY;

	CLine( int _startX, int _startY, int _endX, int _endY ) :
		startX( _startX ),
		startY( _startY ),
		endX( _endX ),
		endY( _endY )
	{
	}

	void Set( int startX, int startY, int endX, int endY ) {
		this->startX = startX;
		this->startY = startY;
		this->endX = endX;
		this->endY = endY;
	}
};

// Что из этой модельки нужно отрисовать на экране
struct CDrawParams {
	std::wstring text;

	std::list<CLine> polygon;

	CDrawParams()
	{
	}

	CDrawParams( std::wstring _text, std::list<CLine> _polygon ) :
		text( _text ),
		polygon( _polygon )
	{
	}
};

// Модель элемента выражения
class IBaseExprModel {
protected:
	std::shared_ptr<IBaseExprModel> parent;
	CRectI rect;
	CDrawParams params;

public:
	virtual ~IBaseExprModel()
	{
	}

	virtual std::shared_ptr<IBaseExprModel> GetParent( ) const;
	virtual void SetParent( std::shared_ptr<IBaseExprModel> parent );

	virtual std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const = 0;

	virtual CRectI GetRect() const;
	virtual void SetRect( CRectI rect );

	// изменение размеров (только размеров) своего прямоугольника в соответствии с размерами прямоугольников непосредственных детей
	virtual void Resize() = 0;

	// расставить детей по своим местам
	virtual void PermutateChildren() = 0;

	virtual CDrawParams GetDrawParams() const;

	virtual ViewType GetType() const = 0;
};

inline std::shared_ptr<IBaseExprModel> IBaseExprModel::GetParent( ) const {
	return parent;
}

inline void IBaseExprModel::SetParent( std::shared_ptr<IBaseExprModel> parent ) {
	this->parent = parent;
}

inline CRectI IBaseExprModel::GetRect( ) const {
	return rect;
}

inline void IBaseExprModel::SetRect( CRectI rect ) {
	this->rect = rect;
}

inline CDrawParams IBaseExprModel::GetDrawParams() const {
	return params;
}