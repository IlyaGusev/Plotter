#pragma once
#include <string>
//#include "calculator.h"

class MathCore {
public:
	MathCore( const std::string& formula ) {};
	MathCore() {};
	~MathCore() {};

	double calculate( double x, double y ) {};

	void changeWindowCoordinates( double x, double y, double z ) {};
	void operator = ( const MathCore& inputMCore ) {};
private:
	const std::string formula;
	//Calculator calc;

	//Number globalXShift;
	//Number globalYShift;
	//Number globalZShift;
};