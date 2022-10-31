#pragma once
#include <functional>
#include <string>

class Operation {
 public:
  std::string symbol_;
  int priority_;
  std::function<double(double, double)> function;

  Operation(Operation&& operation);
  Operation(Operation const& operation);
  Operation(std::string&& symbol, int&& priority,
            std::function<double(double, double)>&& function);
  Operation(std::string const& symbol, int const& priority,
            std::function<double(double, double)> const& function);
};
