#pragma once
#include "Model/IBaseExprModel.h"
#include "Model/ExprControlModel.h"
#include <vector>

// Модель для системы
// Держит на себе вектор ExprControl'ов - "уравнения" системы
class CSystemControlModel : public IBaseExprModel {
public:
	CSystemControlModel( const CRect& rect, std::weak_ptr<IBaseExprModel> parent );

	std::list<std::shared_ptr<IBaseExprModel>> GetChildren() const;

	void InitializeChildren( std::shared_ptr<IBaseExprModel> initChild = 0 );

	void SetRect( const CRect& rect );

	void Resize();

	void PlaceChildren();

	int GetMiddle() const;

	ViewType GetType() const;

	void MoveBy( int dx, int dy );

	void MoveCaretLeft( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	void MoveCaretRight( const IBaseExprModel* from, CCaret& caret, bool isInSelectionMode = false );
	
	bool IsEmpty() const;

	void UpdateSelection();

  int CalcHeight() const;

  void AddChild( int num, std::shared_ptr<IBaseExprModel> initChild = 0 );
  std::shared_ptr<IBaseExprModel> TryRemoveChild(int line); // контрол нового активного edit если ребенок удалился, nullptr иначе

	std::shared_ptr<IBaseExprModel> CopySelected() const;
	std::wstring Serialize();

  int FindLineNum(std::shared_ptr<CEditControlModel>);
  bool CanRemoveChild(int line);
private:
  // Дети-линии
  std::vector<std::shared_ptr<IBaseExprModel>> children;

	void updatePolygons();
};