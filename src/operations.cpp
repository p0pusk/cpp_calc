#include "operations.h"

Operation::Operation(Operation &&operation)
    : symbol_(operation.symbol_),
      priority_(operation.priority_),
      function(std::move(operation.function)) {}

Operation::Operation(Operation const &operation)
    : symbol_(operation.symbol_),
      priority_(operation.priority_),
      function(operation.function) {}

Operation::Operation(std::string &&symbol, int &&priority,
                     std::function<double(double, double)> &&function)
    : symbol_(symbol), priority_(priority), function(std::move(function)) {}

Operation::Operation(std::string const &symbol, int const &priority,
                     std::function<double(double, double)> const &function)
    : symbol_(symbol), priority_(priority), function(function) {}
