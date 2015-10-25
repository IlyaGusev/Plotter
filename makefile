all:
	g++ -std=c++11 FormulaComputability/main.cpp pugixml/pugixml.cpp FormulaComputability/FormulaComputability.cpp FormulaComputability/CTagContainer.cpp FormulaComputability/CTag.cpp FormulaComputability/CTagQualifiers.cpp FormulaComputability/CTagDeclare.cpp FormulaComputability/CTagCi.cpp FormulaComputability/CTagCn.cpp
