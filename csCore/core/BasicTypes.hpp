#pragma once
#include <type_traits>

using float32_t = float;
using float64_t = double;

namespace csCore {
namespace core {

template <class T>
struct is_float32_t : std::integral_constant<bool, std::is_same<float32_t, typename std::remove_cv<T>::type>::value> {};

template <class T>
struct is_float64_t : std::integral_constant<bool, std::is_same<float64_t, typename std::remove_cv<T>::type>::value> {};

template <class T>
struct is_float32_or_64_t : std::integral_constant<bool, std::is_same<float64_t, typename std::remove_cv<T>::type>::value ||
                                                             std::is_same<float32_t, typename std::remove_cv<T>::type>::value> {};

} // namespace core
} // namespace csCore
