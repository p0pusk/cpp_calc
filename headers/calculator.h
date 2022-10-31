#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

#include "operations.h"
#include "parser.h"
#include "plugin_manager.h"

class Calculator {
 private:
  std::map<std::string, Operation> operations_;
  std::shared_ptr<PluginManager> pm_;

  double EvalPostfix(std::vector<Token>& expr);

 public:
  Calculator();
  ~Calculator();
  double Calculate(std::string& input);
};