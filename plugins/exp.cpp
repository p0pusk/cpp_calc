#include <cmath>


extern "C"
{
    double Unary(double x)
    {
        return exp(x);
    }
}