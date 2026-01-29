#pragma once

#include <map>
#include <filesystem>
#include <math.h>
#include <numbers>
#include <cstddef>
#include <functional>

using FEngineProcedure = std::function<void()>;

//Common used types
using string = std::string;
using uint = unsigned int;
using byte = std::byte;
using time_type = double;


using ifstream = std::ifstream;
using stringstream = std::stringstream;
using exception = std::exception;

#include <vector>
template<typename T>
using vector = std::vector<T>;

#include <set>
template<typename T>
using set = std::set<T>;

template<typename Key, typename Value>
using map = std::map<Key,Value>;

//filesystem define
namespace filesystem = std::filesystem;

//math defines
constexpr float pi = (float) std::numbers::pi;

