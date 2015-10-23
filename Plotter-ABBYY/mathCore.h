#pragma once
#include <string>
#include "calculator.h"
#include "./pugixml/pugixml.hpp"


class MathCore {
public:
	//MathCore( const std::string &formula, bool _is2D = false );
	//~MathCore();
	MathCore( const pugi::xml_node& formulaRoot, bool _is2D = false );

	double calculate( double x, double y );

	void changeWindowCoordinates( double x, double y, double z );
	void changeScale( double factor );

	bool Is2D() const;

	double scale;
private:
	//StringCalculator calc;
	MathMlCalculator calc;
	double globalXShift;
	double globalYShift;
	double globalZShift;
	
	bool is2D;
};