#pragma once
#include <vector>
#include <stack>
#include <iostream>
#include <map>
#include <cmath>
#include "parser.h"
#include "operations.h"
#include "pluginManager.h"


class Calculator
{
  private:
    std::map<std::string, Operation> operations;
    std::shared_ptr<PluginManager> pm;

    double EvalPostfix(std::vector<Token> expr);

  public:
    Calculator();
    ~Calculator();
    double Calculate(std::string input);
};