all:
	g++ -std=c++11 FormulaComputability/CCalculator.h FormulaComputability/CCalculator.cpp FormulaComputability/main.cpp pugixml/pugixml.cpp FormulaComputability/FormulaComputability.cpp FormulaComputability/CTagContainer.cpp FormulaComputability/CTag.cpp FormulaComputability/CTagQualifiers.cpp FormulaComputability/CTagDeclare.cpp FormulaComputability/CTagCi.cpp FormulaComputability/CTagCn.cpp
