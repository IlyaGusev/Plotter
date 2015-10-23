#pragma once
#include <string>
#include "calculator.h"
//#include "calculator.h"

class MathCore {
public:
	MathCore( const std::string &formula, bool is2D = false );
	//~MathCore();

	double calculate( double x, double y );

	void changeWindowCoordinates( double x, double y, double z );
	void changeScale( double factor );

	bool Is2D() const;

	double scale;
private:
	Calculator calc;
	double globalXShift;
	double globalYShift;
	double globalZShift;
	
	bool is2D;
};