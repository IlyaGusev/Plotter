#include <functional>
#include <queue>

#include <Model/IBaseExprModel.h>

// класс, который обходит в ширину дерево и совершает над ним необходимые действия
// сделан отдельно метод для удобного поиска
class CTreeBfsProcessor
{
	IBaseExprModel* _startingNode;
	std::function<void( IBaseExprModel* )> _afterEnter;
	std::function<void( IBaseExprModel*, IBaseExprModel* )> _beforeEachChild;
	std::function<bool( IBaseExprModel*, IBaseExprModel* )> _condition;
	std::function<void( IBaseExprModel*, IBaseExprModel* )> _afterEachChild;
	std::function<void( IBaseExprModel* )> _beforeExit;

public:
		// при вызове process процессор начинает обход в ширину с startingNode вниз
		// при входе в вершину node вызывается afterEnter(node)
		// перед тем как положить в очередь ребёнка node, вершину child, выполняется beforeEachChild(node, child)
		// если condition(node, child) = false, то child не попадает в очередь
		// после того как child положили в очередь, выполняется afterEachChild(node, child)
		// перед выходом из вершины выполняется beforeExit(node)
		CTreeBfsProcessor(
			IBaseExprModel* startingNode = nullptr,
			const std::function<void( IBaseExprModel* )>& afterEnter = std::function<void( IBaseExprModel* )>( []( IBaseExprModel* arg1 ){} ),
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

	// функция ищет самую первую в порядке обхода в ширину вершину в дереве, для которой predicate = true
	// обход при этом не затрагивает вершины, для которых hint = false (т.е. и их потомков тоже)
	IBaseExprModel* Find(
		const std::function<bool( IBaseExprModel* )>& predicate,
		const std::function<bool( IBaseExprModel*, IBaseExprModel* )>& hint = std::function<bool( IBaseExprModel*, IBaseExprModel* )>( []( IBaseExprModel* arg1, IBaseExprModel* arg2 ){return true;} ) ) const;

	void Process( ) const;
};