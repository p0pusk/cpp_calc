#include <iostream>
#include <string>
#include "calculator.h"


int main(int argc, char** argv)
{
    std::string input;
    std::cout << ">>> ";
    while (std::getline(std::cin, input) && input != "exit")
    {
        Calculator calc;
        std::cout << "  = " << calc.Calculate(input) << std::endl;
        std::cout << ">>> ";
    }
}
