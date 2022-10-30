#include <iostream>
#include <cmath>


extern "C"
{
    double Unary(double x)
    {
        return sin(x);
    }
}

