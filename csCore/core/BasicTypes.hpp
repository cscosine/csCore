#pragma once
#include <limits>
#include <type_traits>

using float32_t = float;
using float64_t = double;

namespace csCore {
namespace core {

static_assert(std::numeric_limits<float>::is_iec559, "float is not IEEE 754");
static_assert(std::numeric_limits<double>::is_iec559, "double is not IEEE 754");

static_assert(sizeof(float) * 8 == 32, "float is not 32-bit");
static_assert(sizeof(double) * 8 == 64, "double is not 64-bit");

template <class T>
struct is_float32_t : std::integral_constant<bool, std::is_same<float32_t, typename std::remove_cv<T>::type>::value> {};

template <class T>
struct is_float64_t : std::integral_constant<bool, std::is_same<float64_t, typename std::remove_cv<T>::type>::value> {};

template <class T>
struct is_float32_or_64_t
    : std::integral_constant<bool, std::is_same<float64_t, typename std::remove_cv<T>::type>::value ||
                                       std::is_same<float32_t, typename std::remove_cv<T>::type>::value> {};

} // namespace core
} // namespace csCore
