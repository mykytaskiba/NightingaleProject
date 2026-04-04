#pragma once

#include <map>
#include <filesystem>
#include <math.h>
#include <numbers>
#include <cstddef>
#include <functional>

#include "nightingale_assert.h"

using FEngineProcedure = std::function<void()>;

//Common used types
using string = std::string;
using uint = unsigned int;
using byte = std::byte;

//Logic time variables
using TTimeAbs = double;
using TTimeDelta = float;


using ifstream = std::ifstream;
using stringstream = std::stringstream;
using exception = std::exception;


