#include "mathCore.h"
#include <iostream>
#include <exception>

MathCore::MathCore(const std::string &formula) :
	calc(formula),
	globalXShift(0),
	globalYShift(0),
	globalZShift(0)
{
}

MathCore::~MathCore() {
}

double MathCore::calculate(double x, double y) {
	try {
		return calc.countExpression(x + globalXShift, y + globalYShift) - globalZShift;
	}
	catch (std::logic_error catchedException) {
		std::cout << catchedException.what() << ", impossible to count" << '\n';
	}
	return 0;
}

void MathCore::changeWindowCoordinates(double x, double y, double z) {
	globalXShift = globalXShift + x;
	globalYShift = globalYShift + y;
	globalZShift = globalZShift + z;
}