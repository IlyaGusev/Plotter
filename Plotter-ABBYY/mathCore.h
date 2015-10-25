//#pragma once
//#include <string>
//#include <vector>
//
//#include "calculator.h"
//#include "./pugixml/pugixml.hpp"
//
//
//class MathCore {
//public:
//	MathCore() {}
//	MathCore( const std::wstring& formulaPath, bool _is2D = false );
//	//~MathCore();
//	MathCore( const pugi::xml_node& formulaRoot, bool _is2D = false );
//
//	double calculate( double x, double y );
//
//	bool Is2D() const;
//private:
//	// создает сетку в зависимости от размера ячейки
//	void generateGrid();
//
//	//StringCalculator calc;
//};