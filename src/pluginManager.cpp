#include "pluginManager.h"


void* PluginManager::OpenLib(std::string const& func_name)
{
    const std::string plugin_dir_path = "/Users/artemson/polykek/cpp/hw1/build/plugins/";
    std::string path = plugin_dir_path + "lib" + func_name + ".so";
    void* dylib = dlopen(path.c_str(), RTLD_LAZY);

    if (dylib == NULL) {
        throw std::runtime_error("Error loading lib " + path);
    }

    return dylib;
}

void PluginManager::CloseLibs()
{
    while (!handlers.empty())
    {
        dlclose(handlers.top());
        handlers.pop();
    }

    binaryFunctions.clear();
    unaryFunctions.clear();
}

void PluginManager::LoadUnaryFunc(std::string const& func_name)
{
    void *dylib;
    try
    {
        dylib = OpenLib(func_name);
    }
    catch(const std::exception& e)
    {
        dlclose(dylib);
        std::cerr << e.what() << '\n';
        return;
    }

    double (*unaryFunc)(double) = (double (*)(double))dlsym(dylib, "Unary");

    if (unaryFunc == NULL)
    {
        dlclose(dylib);
        throw std::runtime_error("Error loading function");
        return;
    }

    handlers.push(dylib);

    unaryFunctions.emplace(func_name, unaryFunc);

    return;
}

void PluginManager::LoadBinaryFunction(std::string const& func_name)
{
    void *dylib;
    try
    {
        dylib = OpenLib(func_name);
    }
    catch(const std::exception& e)
    {
        dlclose(dylib);
        std::cerr << e.what() << '\n';
        return;
    }

    auto binaryFunc = (double (*)(double, double))dlsym(dylib, "Binary");
    dlclose(dylib);

    if (binaryFunc == NULL)
    {
        throw std::runtime_error("Error loading function");
        dlclose(dylib);
        return;
    }

    handlers.push(dylib);
    binaryFunctions.emplace(func_name, binaryFunc);
    return;
}

bool PluginManager::ContainsBinary(std::string const &func_name)
{
    return binaryFunctions.find(func_name) != binaryFunctions.end();
}

bool PluginManager::ContainsUnary(std::string const &func_name)
{
    return unaryFunctions.find(func_name) != unaryFunctions.end();
}

std::function<double(double)> PluginManager::GetUnaryFunction(std::string const &func_name)
{
    auto search = unaryFunctions.find(func_name);
    if (search == unaryFunctions.end())
        throw std::runtime_error("No such function");

    return search->second;
}

std::function<double(double, double)> PluginManager::GetBinaryFunction(std::string const &func_name)
{
    auto search = binaryFunctions.find(func_name);
    if (search == binaryFunctions.end())
        throw std::runtime_error("No such function");

    return search->second;
}