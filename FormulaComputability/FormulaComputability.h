#pragma once
#ifndef _FORMULACOMPUTALILITY_H
#define _FORMULACOMPUTALILITY_H

#include "CTagContainer.h"
#include "../pugixml/pugixml.hpp"
#include "Tag.h"
#include "TagCi.h"

using namespace pugi;

class FormulaComputability
{
public:
	FormulaComputability();
    static bool checkComputability(const CNode& rootNode);
	~FormulaComputability();
};

#endif