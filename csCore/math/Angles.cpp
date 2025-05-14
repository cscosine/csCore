#include <csCore/core/BasicTypes.hpp>
#include <csCore/csCoreAssert.hpp>
#include <csCore/math/Angles.hpp>
#include <type_traits>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cmath>

namespace csCore {
namespace math {

template <class T, class RetType>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> degToRad(T in) {
  CSCORE_ASSERT_SAFE_MATH(std::isfinite(in));

  return in * static_cast<T>(M_PI) / static_cast<T>(180.0);
}

template <class T, class RetType>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> radToDeg(T in) {
  CSCORE_ASSERT_SAFE_MATH(std::isfinite(in));

  return in / static_cast<T>(M_PI) * static_cast<T>(180.0);
}

template <class T, class RetType>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> wrapToPi(T in) {
  CSCORE_ASSERT_SAFE_MATH(std::isfinite(in));

  return std::atan2(std::sin(in), std::cos(in));
}

template <class T, class RetType>
std::enable_if_t<csCore::core::is_float32_or_64_t<T>::value, RetType> angleDistance(T reference, T target) {
  CSCORE_ASSERT_SAFE_MATH(std::isfinite(reference));
  CSCORE_ASSERT_SAFE_MATH(std::isfinite(target));

  // equivalent to create R(a)^T * R(b) and then take the log, but without
  // matrices

  T const ca{std::cos(reference)};
  T const sa{std::sin(reference)};
  T const cb{std::cos(target)};
  T const sb{std::sin(target)};

  T const c{ca * cb + sa * sb};
  T const s{-sa * cb + ca * sb};

  return std::atan2(s, c);
}

template std::enable_if_t<csCore::core::is_float32_or_64_t<float32_t>::value, float32_t> degToRad<float32_t, float32_t>(float32_t);
template std::enable_if_t<csCore::core::is_float32_or_64_t<float64_t>::value, float64_t> degToRad<float64_t, float64_t>(float64_t);

template std::enable_if_t<csCore::core::is_float32_or_64_t<float32_t>::value, float32_t> radToDeg<float32_t, float32_t>(float32_t);
template std::enable_if_t<csCore::core::is_float32_or_64_t<float64_t>::value, float64_t> radToDeg<float64_t, float64_t>(float64_t);

template std::enable_if_t<csCore::core::is_float32_or_64_t<float32_t>::value, float32_t> wrapToPi<float32_t, float32_t>(float32_t);
template std::enable_if_t<csCore::core::is_float32_or_64_t<float64_t>::value, float64_t> wrapToPi<float64_t, float64_t>(float64_t);

template std::enable_if_t<csCore::core::is_float32_or_64_t<float32_t>::value, float32_t>
    angleDistance<float32_t, float32_t>(float32_t, float32_t);
template std::enable_if_t<csCore::core::is_float32_or_64_t<float64_t>::value, float64_t>
    angleDistance<float64_t, float64_t>(float64_t, float64_t);

} // namespace math
} // namespace csCore
