#include <csCore/math/AnglesConversion.hpp>

#include <csCore/core/BasicTypes.hpp>

#include <csCore/csCoreAssert.hpp>

#include <type_traits>

#define _USE_MATH_DEFINES
#include <math.h>

#include <cmath>

namespace csCore {
    namespace math {

        template <class T> T degToRad(T in) {
            CSCORE_CONTRACT_DEFENSIVE(std::isfinite(in));

            return in * static_cast<T>(M_PI) / static_cast<T>(180.0);
        }

        template <class T> T radToDeg(T in) {
            CSCORE_CONTRACT_DEFENSIVE(std::isfinite(in));

            return in / static_cast<T>(M_PI) * static_cast<T>(180.0);
        }

        template float32_t degToRad<float32_t>(float32_t);
        template float64_t degToRad<float64_t>(float64_t);

        template float32_t radToDeg<float32_t>(float32_t);
        template float64_t radToDeg<float64_t>(float64_t);

    }
}

