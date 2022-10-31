#include "plugin_manager.h"

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

  functions_.clear();
}

void PluginManager::LoadFunction(std::string const& func_name) {
  void* dylib;
  try {
    dylib = OpenLib(func_name);
  } catch (const std::exception& e) {
    dlclose(dylib);
    std::cerr << e.what() << '\n';
    return;
  }

  double (*func)(double) = (double (*)(double))dlsym(dylib, "Unary");

  if (func == NULL) {
    dlclose(dylib);
    throw std::runtime_error("Error loading function");
    return;
  }

  handlers_.push(dylib);

  functions_.emplace(func_name, func);

  return;
}

bool PluginManager::ContainsFunction(std::string const& func_name) {
  return functions_.find(func_name) != functions_.end();
}

std::function<double(double)> PluginManager::GetFunction(
    std::string const& func_name) {
  auto search = functions_.find(func_name);
  if (search == functions_.end()) throw std::runtime_error("No such function");

  return search->second;
}