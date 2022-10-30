#include "operations.h"

Operation::Operation(Operation &&operation)
    :symbol(operation.symbol), 
    priority(operation.priority), 
    associativity(std::move(operation.associativity)),
    function(std::move(operation.function)) { }

Operation::Operation(Operation const& operation)
    :symbol(operation.symbol),
    priority(operation.priority),
    associativity(operation.associativity),
    function(operation.function) {  }

Operation::Operation(std::string &&symbol, int &&priority, Associativity &&associativity,
            std::function<double(double, double)> &&function)
    :symbol(symbol), priority(priority), associativity(std::move(associativity)), 
    function(std::move(function)) { }

Operation::Operation(std::string const &symbol, int const &priority, Associativity const &associativity, 
              std::function<double(double, double)> const &function)
    :symbol(symbol), priority(priority), associativity(associativity), function(function) { }
