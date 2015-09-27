#pragma once
#include "CTagContainer.h"
#include "../pugixml/pugixml.hpp"
#include "Tag.h"

using namespace pugi;

class FormulaComputability
{
public:
	FormulaComputability();
    static bool checkComputability(const CNode& rootNode);
	~FormulaComputability();
};

