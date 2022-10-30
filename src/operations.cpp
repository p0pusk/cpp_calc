#include "operations.h"

Operation::Operation(Operation &&operation)
    : symbol(operation.symbol), 
      priority(operation.priority), 
      function(std::move(operation.function)) {}

Operation::Operation(Operation const& operation)
    : symbol(operation.symbol),
      priority(operation.priority),
      function(operation.function) {}

Operation::Operation(std::string &&symbol, int &&priority, 
                     std::function<double(double, double)> &&function)
    : symbol(symbol), priority(priority), function(std::move(function)) {}

Operation::Operation(std::string const &symbol, int const &priority,
std::function<double(double, double)> const &function)
    : symbol(symbol), priority(priority), function(function) {}
