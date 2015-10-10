all:
	g++ -std=c++11 FormulaComputability/main.cpp pugixml/pugixml.cpp FormulaComputability/FormulaComputability.cpp FormulaComputability/CTagContainer.cpp FormulaComputability/Tag.cpp FormulaComputability/TagQualifiers.cpp FormulaComputability/TagDeclare.cpp FormulaComputability/TagCi.cpp
