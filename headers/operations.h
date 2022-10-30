#pragma once
#include <string>
#include <functional>

enum Associativity
{
    Left,
    Right
};

class Operation
{
  public:
    std::string symbol;
    int priority;
    Associativity associativity;
    std::function<double(double, double)> function;

    Operation(Operation &&operation);
    Operation(Operation const &operation);
    Operation(std::string &&symbol, int &&priority, Associativity &&associativity,
              std::function<double(double, double)> &&function);
    Operation(std::string const &symbol, int const &priority, Associativity const &associativity,
              std::function<double(double, double)> const &function);
};
