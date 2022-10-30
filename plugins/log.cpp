#include <cmath>
#include <stdexcept>


extern "C"
{
    double Unary(double x)
    {
        if (x < 0)
            throw std::runtime_error("Log arg is greater than zero");

        return log(x);
    }
}