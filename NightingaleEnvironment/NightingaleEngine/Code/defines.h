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


//NO_HEAP_ALLOCATION
//Define inside of a class to remove that class from ever being on the heap
//#define NO_HEAP_ALLOCATION                              \
    private:                                            \
    static void* operator new(std::size_t size) {}      \
    static void* operator new[](std::size_t size) {}    



//Select the level of logging to be done
//NONE -> No Logs are produced
//LITE -> Logs 
//#define LOGGING_NONE
//#define LOGGING_LITE
//#define LOGGING_FULL