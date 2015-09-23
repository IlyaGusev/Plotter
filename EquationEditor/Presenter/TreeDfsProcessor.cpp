#include <Presenter/TreeDfsProcessor.h>

CTreeDfsProcessor::CTreeDfsProcessor(
	IBaseExprModel* startingNode,
	const std::function<void( IBaseExprModel* )>& afterEnter,
	const std::function<void( IBaseExprModel*, IBaseExprModel* )>& beforeEachChild,
	const std::function<bool( IBaseExprModel*, IBaseExprModel* )>& condition,
	const std::function<void( IBaseExprModel*, IBaseExprModel* )>& afterEachChild,
	const std::function<void( IBaseExprModel* )>& beforeExit) :
	_startingNode( startingNode ),
	_afterEnter( afterEnter ),
	_beforeEachChild( beforeEachChild ),
	_condition( condition ),
	_afterEachChild( afterEachChild ),
	_beforeExit( beforeExit )
{
}

void CTreeDfsProcessor::SetStartingNode( IBaseExprModel* startingNode )
{
	_startingNode = startingNode;
}

void CTreeDfsProcessor::SetEnterProcessFunc( const std::function<void( IBaseExprModel* )>& afterEnter )
{
	_afterEnter = afterEnter;
}

void CTreeDfsProcessor::SetBeforeChildProcessFunc( const std::function<void( IBaseExprModel*, IBaseExprModel* )>& beforeEachChild )
{
	_beforeEachChild = beforeEachChild;
}

void CTreeDfsProcessor::SetCondition( const std::function<bool(IBaseExprModel*, IBaseExprModel*)>& condition )
{
	_condition = condition;
}

void CTreeDfsProcessor::SetAfterChildProcessFunc( const std::function<void( IBaseExprModel*, IBaseExprModel* )>& afterEachChild )
{
	_afterEachChild = afterEachChild;
}

void CTreeDfsProcessor::SetExitProcessFunc( const std::function<void( IBaseExprModel* )>& beforeExit )
{
	_beforeExit = beforeExit;
}

void CTreeDfsProcessor::dfsFunction( IBaseExprModel* currentNode ) const
{
	_afterEnter(currentNode);
	for (auto child : currentNode->GetChildren())
	{
		_beforeEachChild(currentNode, child);
		if (_condition( currentNode, child ))
		{
			dfsFunction( child );
		}
		_afterEachChild(currentNode, child);
	}
	_beforeExit(currentNode);
}

IBaseExprModel* CTreeDfsProcessor::Find(
	const std::function<bool(IBaseExprModel*)>& predicate,
	const std::function<bool(IBaseExprModel*, IBaseExprModel*)>& hint ) const
{
	return nullptr;
}

void CTreeDfsProcessor::Process( ) const
{
	if (_startingNode == nullptr)
	{
		throw std::exception( "starting node is not initialized" );
	}
	dfsFunction( _startingNode );
}