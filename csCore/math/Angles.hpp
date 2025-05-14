#pragma once
#include <csCore/core/BasicTypes.hpp>

namespace csCore {
namespace math {

template <class T, class RetType = T>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> degToRad(T in);

template <class T, class RetType = T>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> radToDeg(T in);

template <class T, class RetType = T>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> wrapToPi(T in);

/// @return minimum angle target - reference, considering wrapping. Equivalent
/// to log(R(reference)^T * R(target))
template <class T, class RetType = T>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> angleDistance(T reference, T target);

} // namespace math
} // namespace csCore
