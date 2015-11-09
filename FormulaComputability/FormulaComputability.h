#pragma once
#ifndef _FORMULACOMPUTALILITY_H
#define _FORMULACOMPUTALILITY_H

#include "CTagContainer.h"
#include "../pugixml/pugixml.hpp"
#include "CTag.h"
#include "CTagCi.h"
#include "CCalculator.h"
#include "CTree.h"


using namespace pugi;

//основной класс, проверяющмй вычислимость формулы
class FormulaComputability
{
public:
	FormulaComputability();
    static bool checkComputability(const CNode& rootNode);//проверяет вычислимость формулы,
                                                        //rootNode - корневой элемент xml-документа
	static bool solveQudraticEquation(const CNode& rootNode);
	static bool calculateLimits(const CNode& rootNode, string);
	~FormulaComputability();
};

#endif
