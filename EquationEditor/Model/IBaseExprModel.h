#pragma once
#include <list>
#include <memory>
#include <Windows.h>

#include "Model/Utils/Rect.h"
#include "Model/Utils/Line.h"
#include "Model/Utils/Caret.h"

// Реализованные типы вьюшек
enum ViewType { TEXT, EXPR, FRAC, DEGR, SUBSCRIPT, RADICAL, PARENTHESES };

// Что из этой модельки нужно отрисовать на экране
struct CDrawParams {
	std::wstring text;			// Текст, содержащийся в модельке
								// Имеет смысл начать хранить текст посимвольно с информацией
								// об относящемуся к каждому символу стилю (курсив ли, размер, бэкграунд и цвет)
	std::pair<int, int> selectedPositions;		// Номера первой и следующей за последней выбранной буквы

	std::list<CLine> polygon;	// Набор относящихся к модели линий, рисуемых на экране
	bool isHighlighted;			// Есть ли подсветка (должна быть над созданным контролом до того, как в него что-то введут)
	bool isSelected;			// Выделена ли вьюшка

	CDrawParams() :
		isHighlighted( false ),
		isSelected( false )
	{
		selectedPositions.first = selectedPositions.second = 0;
	}
};

// Модель элемента выражения
class IBaseExprModel : public std::enable_shared_from_this<IBaseExprModel> {
protected:
	std::weak_ptr<IBaseExprModel> parent;
	CRect rect;		// Стоит подумать о переносе в CDrawParams
	CDrawParams params;
	int depth;		// Расстояние до корня в дереве

	IBaseExprModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent, int depth = 0 ) :
		parent( parent ),
		rect( rect ),
		depth( depth )
	{
	}
public:
	virtual ~IBaseExprModel()
	{
	}

	virtual std::weak_ptr<IBaseExprModel> GetParent() const;
	virtual void SetParent( const std::weak_ptr<IBaseExprModel> parent );

	virtual std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const = 0;

	// Прикрепляет детей к свежесозданной модели. Имеет смысл вызывать эту функцию после каждого конструктора
	virtual void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 ) = 0;

	// Нужно вызывать при создании модели для корректного определения начального положения дочерних элементов
	virtual void SetRect( const CRect& rect );
	// Просто выставляет размеры прямоугольника
	virtual CRect GetRect();
	// Двигает прямоугольник со всеми относящимися к нему дочерними элементами 
	virtual void MoveBy( int dx, int dy );

	// изменение размеров (только размеров) своего прямоугольника в соответствии с размерами прямоугольников непосредственных детей
	virtual void Resize() = 0;

	// расставить детей по своим местам
	virtual void PlaceChildren() = 0;

	// выдаёт середину модели, по которой будет выполняться выравнивание
	virtual int GetMiddle() const = 0;

	// Возвращает текст, хранящийся в этой модели
	virtual std::list<std::pair<std::wstring, CRect>> GetSelectedText() const;
	virtual std::list<std::pair<std::wstring, CRect>> GetUnselectedText() const;

	// Возвращает набор линий, которые нужно провести на вьюшке, относящейся к этой модели
	virtual std::list<CLine> GetLines() const;

	// Говорит, подсвечен ли прямоугольник этого контрола
	virtual bool IsHighlighted() const;
	virtual void SetHighlighting( bool newHighlighting );
	
	// Говорит, выделен ли прямоугольник
	virtual bool IsSelected() const;
	virtual void DeleteSelection();
	// Обновляет поле selection в соответствии со своими детьми: если все дети выделены, то и их родитель выделен
	virtual void UpdateSelection() = 0;

	// Удаляет тот кусок, который помечен, как isSelected
	// Возвращает false, когда требуется дополнительная обработка от родителя
	virtual bool DeleteSelectedPart();

	// Возвращает тип модели
	virtual ViewType GetType() const = 0;

	// Сдвигает каретку в нужную сторону относительно from
	virtual void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false ) = 0;
	virtual void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false ) = 0;

	virtual bool IsEmpty() const = 0;

	virtual int GetDepth() const;
	virtual void UpdateDepth();

	// Отдает копию поддерева со всеми вершинами, помеченными как selected
	virtual std::shared_ptr<IBaseExprModel> CopySelected() const = 0;
};

inline std::weak_ptr<IBaseExprModel> IBaseExprModel::GetParent( ) const
{
	return parent;
}

inline void IBaseExprModel::SetParent( const std::weak_ptr<IBaseExprModel> parent )
{
	this->parent = parent;
}

inline CRect IBaseExprModel::GetRect()
{
	return rect;
}

inline void IBaseExprModel::SetRect( const CRect& rect )
{
	this->rect = rect;
}

inline void IBaseExprModel::MoveBy( int dx, int dy ) 
{
	rect.MoveBy( dx, dy );
}

inline std::list<CLine> IBaseExprModel::GetLines() const 
{
	return params.polygon;
}

inline bool IBaseExprModel::IsHighlighted() const
{
	return params.isHighlighted;
}

inline void IBaseExprModel::SetHighlighting( bool newHighlighting )
{
	params.isHighlighted = newHighlighting;
}

inline bool IBaseExprModel::IsSelected() const {
	return params.isSelected;
}

inline std::list<std::pair<std::wstring, CRect>> IBaseExprModel::GetSelectedText( ) const 
{
	return std::list<std::pair<std::wstring, CRect>>();
}

inline std::list<std::pair<std::wstring, CRect>> IBaseExprModel::GetUnselectedText( ) const 
{
	return std::list<std::pair<std::wstring, CRect>>();
}

inline void IBaseExprModel::DeleteSelection() {
	params.isSelected = false;
}

inline int IBaseExprModel::GetDepth() const {
	return depth;
}

inline bool IBaseExprModel::DeleteSelectedPart() {
	auto children = GetChildren();
	bool result = true;
	for( auto it = children.begin(); it != children.end(); ++it ) {
		// Если нужно дополнительное вмешательство
		result &= (*it)->DeleteSelectedPart();
	}
	return result;
}

inline void IBaseExprModel::UpdateDepth() {
	if( !parent.expired() ) {
		depth = parent.lock()->depth + 1;
	}
	for( auto child : GetChildren() ) {
		child->UpdateDepth();
	}
}
