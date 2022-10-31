#include "pluginManager.h"

void* PluginManager::OpenLib(std::string const& func_name) {
  const std::string plugin_dir_path =
      "/Users/artemson/polykek/cpp/hw1/build/plugins/";
  std::string path = plugin_dir_path + "lib" + func_name + ".so";
  void* dylib = dlopen(path.c_str(), RTLD_LAZY);

  if (dylib == NULL) {
    throw std::runtime_error("Error loading lib " + path);
  }

  return dylib;
}

void PluginManager::CloseLibs() {
  while (!handlers_.empty()) {
    dlclose(handlers_.top());
    handlers_.pop();
  }

  binary_functions_.clear();
  unary_functions_.clear();
}

void PluginManager::LoadUnaryFunc(std::string const& func_name) {
  void* dylib;
  try {
    dylib = OpenLib(func_name);
  } catch (const std::exception& e) {
    dlclose(dylib);
    std::cerr << e.what() << '\n';
    return;
  }

  double (*unaryFunc)(double) = (double (*)(double))dlsym(dylib, "Unary");

  if (unaryFunc == NULL) {
    dlclose(dylib);
    throw std::runtime_error("Error loading function");
    return;
  }

  handlers_.push(dylib);

  unary_functions_.emplace(func_name, unaryFunc);

  return;
}

void PluginManager::LoadBinaryFunction(std::string const& func_name) {
  void* dylib;
  try {
    dylib = OpenLib(func_name);
  } catch (const std::exception& e) {
    dlclose(dylib);
    std::cerr << e.what() << '\n';
    return;
  }

  auto binaryFunc = (double (*)(double, double))dlsym(dylib, "Binary");
  dlclose(dylib);

  if (binaryFunc == NULL) {
    throw std::runtime_error("Error loading function");
    dlclose(dylib);
    return;
  }

  handlers_.push(dylib);
  binary_functions_.emplace(func_name, binaryFunc);
  return;
}

bool PluginManager::ContainsBinary(std::string const& func_name) {
  return binary_functions_.find(func_name) != binary_functions_.end();
}

bool PluginManager::ContainsUnary(std::string const& func_name) {
  return unary_functions_.find(func_name) != unary_functions_.end();
}

std::function<double(double)> PluginManager::GetUnaryFunction(
    std::string const& func_name) {
  auto search = unary_functions_.find(func_name);
  if (search == unary_functions_.end())
    throw std::runtime_error("No such function");

  return search->second;
}

std::function<double(double, double)> PluginManager::GetBinaryFunction(
    std::string const& func_name) {
  auto search = binary_functions_.find(func_name);
  if (search == binary_functions_.end())
    throw std::runtime_error("No such function");

  return search->second;
}