#pragma once
#include <string>
#include "calculator.h"
//#include "calculator.h"

class MathCore {
public:
	MathCore( const std::string &formula, bool is2D = false );
	//~MathCore();

	double calculate( double x, double y );

	bool Is2D() const;
private:
	Calculator calc;
	bool is2D;
};