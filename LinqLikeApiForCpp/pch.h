#include <algorithm>
#include <type_traits>
#include <numeric>
#include <vector>

#ifdef _MSC_VER
#define NOMINMAX
#include <Windows.h>
#include <vccorlib.h>

#ifdef __cplusplus_winrt
#include <collection.h>
#endif
#endif