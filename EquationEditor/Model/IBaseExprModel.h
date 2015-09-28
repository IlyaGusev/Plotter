#pragma once
#include <list>
#include <memory>

#include "Model/Utils/Rect.h"
#include "Model/Utils/Line.h"

#define MIN(x, y) x < y ? x : y;
#define MAX(x, y) x > y ? x : y;

enum ViewType { TEXT, EXPR, FRAC, DEGR };

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
	CRect rect;
	CDrawParams params;

public:
	virtual ~IBaseExprModel()
	{
	}

	virtual std::shared_ptr<IBaseExprModel> GetParent( ) const;
	virtual void SetParent( std::shared_ptr<IBaseExprModel> parent );

	virtual std::list<std::shared_ptr<IBaseExprModel>> GetChildren( ) const = 0;

	// Нужно вызывать при создании модели для корректного определения начального положения дочерних элементов
	virtual void SetRect( CRect rect );
	// Просто выставляет размеры прямоугольника
	virtual CRect& Rect();
	// Двигает прямоугольник со всеми относящимися к нему дочерними элементами 
	virtual void MoveBy( int dx, int dy );

	// изменение размеров (только размеров) своего прямоугольника в соответствии с размерами прямоугольников непосредственных детей
	virtual void Resize() = 0;

	// расставить детей по своим местам
	virtual void PermutateChildren() = 0;

	// выдаёт середину модели, по которой будет выполняться выравнивание
	virtual int GetMiddle() const = 0;

	virtual CDrawParams GetDrawParams() const;

	virtual ViewType GetType() const = 0;
};

inline std::shared_ptr<IBaseExprModel> IBaseExprModel::GetParent( ) const
{
	return parent;
}

inline void IBaseExprModel::SetParent( std::shared_ptr<IBaseExprModel> parent )
{
	this->parent = parent;
}

inline CRect& IBaseExprModel::Rect( )
{
	return rect;
}

inline void IBaseExprModel::SetRect( CRect rect )
{
	this->rect = rect;
}

inline CDrawParams IBaseExprModel::GetDrawParams() const
{
	return params;
}

inline void IBaseExprModel::MoveBy( int dx, int dy ) {
	rect.MoveBy( dx, dy );
}