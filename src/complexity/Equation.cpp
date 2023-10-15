/**
 * @file ComplexityEquation.cpp
 *
 * $URL$
 * $Id$
 */

#include "Equation.h"

namespace complexity {

const char* LinearEquation::names_[] = {"1", "n"};

const char* BigOLinearEquation::names_[] = {"n"};

const char* QuadEquation::names_[] = {"1", "n", "n^2"};

const char* BigOQuadEquation::names_[] = {"n^2"};

const char* NLogN2Equation::names_[] = {"1", "n", "n*log(n)", "n*log(n)^2"};

const char* BigONLogN2Equation::names_[] = {"n*log(n)^2"};

}  // namespace complexity
