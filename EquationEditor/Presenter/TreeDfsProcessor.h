#include <functional>

#include <Model/IBaseExprModel.h>

// класс, который обходит в глубину дерево и совершает над ним необходимые действия
// сделан отдельно метод для удобного поиска
class CTreeDfsProcessor
{
	IBaseExprModel* _startingNode;
	std::function<void( IBaseExprModel*)> _afterEnter;
	std::function<void( IBaseExprModel*, IBaseExprModel*)> _beforeEachChild;
	std::function<bool( IBaseExprModel*, IBaseExprModel* )> _condition;
	std::function<void( IBaseExprModel*, IBaseExprModel* )> _afterEachChild;
	std::function<void( IBaseExprModel* )> _beforeExit;

	void dfsFunction( IBaseExprModel* currentNode ) const;

public:
	// при вызове process процессор начинает обход в глубину с startingNode
	// при входе в вершину node вызывается afterEnter(node)
	// перед рекурсивным входом в ребёнка node, вершину child, выполняется beforeEachChild(node, child)
	// если condition(node, child) = false, то заход в child не происходит
	// после выхода из child выполняется afterEachChild(node, child)
	// перед выходом из вершины выполняется beforeExit(node)
	CTreeDfsProcessor(
		IBaseExprModel* startingNode = nullptr, 
		const std::function<void( IBaseExprModel* )>& afterEnter = std::function<void( IBaseExprModel* )>( [](IBaseExprModel* arg1){} ),
		const std::function<void( IBaseExprModel*, IBaseExprModel* )>& beforeEachChild = std::function<void( IBaseExprModel*, IBaseExprModel* )>( []( IBaseExprModel* arg1, IBaseExprModel* arg2 ){} ),
		const std::function<bool( IBaseExprModel*, IBaseExprModel* )>& condition = std::function<bool( IBaseExprModel*, IBaseExprModel* )>( []( IBaseExprModel* arg1, IBaseExprModel* arg2 ){return true;} ),
	const std::function<void( IBaseExprModel*, IBaseExprModel* )>& afterEachChild = std::function<void( IBaseExprModel*, IBaseExprModel* )>( []( IBaseExprModel* arg1, IBaseExprModel* arg2 ){} ),
	const std::function<void( IBaseExprModel* )>& beforeExit = std::function<void( IBaseExprModel* )>( []( IBaseExprModel* arg1 ){} ) );

	void SetStartingNode( IBaseExprModel* startingNode );
	void SetEnterProcessFunc( const std::function<void( IBaseExprModel* )>& afterEnter );
	void SetBeforeChildProcessFunc( const std::function<void( IBaseExprModel*, IBaseExprModel* )>& beforeEachChild );
	void SetCondition( const std::function<bool( IBaseExprModel*, IBaseExprModel* )>& condition );
	void SetAfterChildProcessFunc( const std::function<void( IBaseExprModel*, IBaseExprModel* )>& afterEachChild );
	void SetExitProcessFunc( const std::function<void( IBaseExprModel* )>& beforeExit );

	// функция ищет самую верхнюю вершину в дереве, для которой predicate = true
	// обход при этом не затрагивает вершины, для которых hint = false
	IBaseExprModel* Find(
		const std::function<bool( IBaseExprModel* )>& predicate,
		const std::function<bool( IBaseExprModel*, IBaseExprModel* )>& hint = std::function<bool( IBaseExprModel*, IBaseExprModel* )>( []( IBaseExprModel* arg1, IBaseExprModel* arg2 ){return true;} ) ) const;

	void Process() const;
};