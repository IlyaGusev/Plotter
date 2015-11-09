#ifndef _FORMULACOMPUTALILITY_CPP
#define _FORMULACOMPUTALILITY_CPP

#include "FormulaComputability.h"

FormulaComputability::FormulaComputability()
{
}

FormulaComputability::~FormulaComputability()
{
}

bool FormulaComputability::checkComputability( const CNode& rootNode )
{
    CNode currentNode = rootNode.first_child();
    while ( !currentNode.empty() )
    {
        CTag& currentTag = CTagContainer::getTag(currentNode.name());
        CNode nextNode = currentTag.checkSignature( currentNode );
        currentTag(currentNode);
        currentNode = nextNode;
    }
    return true;
}

bool FormulaComputability::solveQudraticEquation( const CNode& rootNode)
{
    CTree tree("quad");
    CNode currentNode = rootNode.first_child();
    while ( !currentNode.empty() ){
        CTag& currentTag = CTagContainer::getTag(currentNode.name());
        CNode nextNode = currentTag.checkSignature( currentNode );
        currentTag(currentNode, tree.root);
        currentNode = nextNode;
    }
    tree.setCoefQuad();
    auto answer = CCalculator::quadraticEquation(tree.coef1, tree.coef2, tree.coef3, tree.coef4);
    cout<<answer.first<<" " <<answer.second<<endl;
    return true;
}

bool FormulaComputability::calculateLimits( const CNode& rootNode, string type)
{
    CTree tree(type);
    CNode currentNode = rootNode.first_child();
    while ( !currentNode.empty() ){
        CTag& currentTag = CTagContainer::getTag(currentNode.name());
        CNode nextNode = currentTag.checkSignature( currentNode );
        currentTag(currentNode, tree.root);
        currentNode = nextNode;
    }
    tree.setCoefLimits();
    int answer;
    if (type=="sum")
        answer = CCalculator::sum(tree.coef1, tree.coef2);
    if (type=="prod")
        answer = CCalculator::prod(tree.coef1, tree.coef2);
    cout<<answer<<endl;
    return true;
}

#endif
