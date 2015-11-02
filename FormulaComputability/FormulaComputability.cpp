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

double FormulaComputability::solveQudraticEquation( const CNode& rootNode)
{
    CTree tree;
    CNode currentNode = rootNode.first_child();
    while ( !currentNode.empty() ){
        CTag& currentTag = CTagContainer::getTag(currentNode.name());
        CNode nextNode = currentTag.checkSignature( currentNode );
        currentTag(currentNode, tree.root);
        currentNode = nextNode;
    }
    tree.setCoef();
    auto answer = CCalculator::quadraticEquation(tree.coef1, tree.coef2, tree.coef3, tree.coef4);
    cout<<answer.first<<" " <<answer.second<<endl;
    return true;
}

#endif
