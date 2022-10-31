#pragma once
#include <dlfcn.h>
#include <iostream>
#include <map>
#include <stack>
#include <stdexcept>

#include "operations.h"

class PluginManager {
 public:
  PluginManager(PluginManager const&) = delete;
  PluginManager& operator=(PluginManager const&) = delete;
  void LoadFunction(std::string const& func_name);

  static std::shared_ptr<PluginManager> Instance() {
    static std::shared_ptr<PluginManager> instance{new PluginManager()};
    return instance;
  }

  std::function<double(double)> GetFunction(std::string const& func_name);
  bool ContainsFunction(std::string const& func_name);
  void CloseLibs();

 private:
  std::stack<void*> handlers_;
  std::map<std::string, std::function<double(double)>> functions_;

  PluginManager(){};
  void* OpenLib(std::string const& func_name);
};
