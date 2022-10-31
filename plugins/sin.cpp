#include <cmath>
#include <iostream>

extern "C" {
double Unary(double x) { return sin(x); }
}
