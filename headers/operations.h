#pragma once
#include <string>
#include <functional>


class Operation
{
  public:
    std::string symbol;
    int priority;
    std::function<double(double, double)> function;

    Operation(Operation &&operation);
    Operation(Operation const &operation);
    Operation(std::string &&symbol, int &&priority,
              std::function<double(double, double)> &&function);
    Operation(std::string const &symbol, int const &priority,
              std::function<double(double, double)> const &function);
};
