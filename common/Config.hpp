#pragma once

#include <ostream>

#if defined(AOUT_PLATFORM_WINDOWS)
        #if defined(AOUT_EXPORTS)
                #define AOUT_API __declspec(dllexport)
        #else
                #define AOUT_API __declspec(dllimport)
        #endif
#elif defined(AOUT_PLATFORM_LINUX)
        #if __GNUC__ >= 4
                #define AOUT_API __attribute__((__visibility__("default")))
        #else
                #define AOUT_API
        #endif
#else
        #error Unsupported platform
#endif

namespace aout {

constexpr unsigned versionMajor = 0;
constexpr unsigned versionMinor = 1;
constexpr unsigned versionPatch = 0;

AOUT_API std::ostream& printVersion(std::ostream& os);

}
