#pragma once

#include <assert.h>

#ifdef _MSC_VER
#define assert(c) if (!(c)) __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define assert(c) if (!(c)) __builtin_trap()
#else
#define assert(c) assert(c) // Fallback to standard assert
#endif