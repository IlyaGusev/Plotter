#include "Presenter/TreeBfsProcessor.h"

CTreeBfsProcessor::CTreeBfsProcessor(
	IBaseExprModel* startingNode,
	const std::function<void( IBaseExprModel* )>& afterEnter,
	const std::function<void( IBaseExprModel*, IBaseExprModel* )>& beforeEachChild,
	const std::function<bool( IBaseExprModel*, IBaseExprModel* )>& condition,
	const std::function<void( IBaseExprModel*, IBaseExprModel* )>& afterEachChild,
	const std::function<void( IBaseExprModel* )>& beforeExit ) :
	_startingNode( startingNode ),
	_beforeEachChild( beforeEachChild ),
	_condition( condition ),
	_afterEachChild( afterEachChild ),
	_beforeExit( beforeExit )
{
}

void CTreeBfsProcessor::SetStartingNode( IBaseExprModel* startingNode )
{
	_startingNode = startingNode;
}

void CTreeBfsProcessor::SetEnterProcessFunc( const std::function<void(IBaseExprModel*)>& afterEnter )
{
	_afterEnter = afterEnter;
}

void CTreeBfsProcessor::SetBeforeChildProcessFunc( const std::function<void(IBaseExprModel*, IBaseExprModel*)>& beforeEachChild )
{
	_beforeEachChild = beforeEachChild;
}

void CTreeBfsProcessor::SetCondition( const std::function<bool(IBaseExprModel*, IBaseExprModel*)>& condition )
{
	_condition = condition;
}

void CTreeBfsProcessor::SetAfterChildProcessFunc( const std::function<void(IBaseExprModel*, IBaseExprModel*)>& afterEachChild )
{
	_afterEachChild = afterEachChild;
}

void CTreeBfsProcessor::SetExitProcessFunc( const std::function<void(IBaseExprModel*)>& beforeExit )
{
	_beforeExit = beforeExit;
}

IBaseExprModel* CTreeBfsProcessor::Find( const std::function<bool(IBaseExprModel*)>& predicate, const std::function<bool(IBaseExprModel*, IBaseExprModel*)>& hint ) const
{
	return nullptr;
}

void CTreeBfsProcessor::Process( ) const
{
	if (_startingNode == nullptr)
	{
		throw std::exception( "starting node is not initialized" );
	}
	
	std::queue<IBaseExprModel*> nodeQueue;
	nodeQueue.push( _startingNode );
	while (nodeQueue.size())
	{
		auto currentNode = nodeQueue.front();
		nodeQueue.pop();
		_afterEnter( currentNode );
		for (auto child : currentNode->GetChildren())
		{
			_beforeEachChild( currentNode, child );
			if (_condition( currentNode, child ))
			{
				nodeQueue.push( child );
			}
			_afterEachChild( currentNode, child );
		}
		_beforeExit( currentNode );
	}
}