#pragma once
#include <iostream>
#include <stdexcept>
#include <dlfcn.h>
#include <stack>
#include <map>
#include "operations.h"


class PluginManager
{
  public:
    PluginManager(PluginManager const&) = delete;
    PluginManager &operator=(PluginManager const&) = delete;
    void LoadUnaryFunc(std::string const &func_name);
    void LoadBinaryFunction(std::string const &func_name);

    static std::shared_ptr<PluginManager> Instance()
    {
      static std::shared_ptr<PluginManager> instance {new PluginManager()};
      return instance;
    }

    std::function<double(double)> GetUnaryFunction(std::string const &func_name);
    std::function<double(double, double)> GetBinaryFunction(std::string const &func_name);
    bool ContainsBinary(std::string const &func_name);
    bool ContainsUnary(std::string const &func_name);

  private:
    std::stack<void *> handlers;
    std::map<std::string, std::function<double(double, double)>> binaryFunctions;
    std::map<std::string, std::function<double(double)>> unaryFunctions;

    PluginManager() {};
    void *OpenLib(std::string const &func_name);
};

