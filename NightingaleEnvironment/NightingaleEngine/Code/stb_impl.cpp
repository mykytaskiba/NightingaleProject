#include "pch.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // Forcefully silence the "unsafe" warning
#endif

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifdef _MSC_VER
#pragma warning(pop) // Restore warnings for anything else in this file
#endif