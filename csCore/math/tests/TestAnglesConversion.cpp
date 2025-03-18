#include <catch2/catch_all.hpp>

#include <csCore/math/AnglesConversion.hpp>
#include <csCore/core/BasicTypes.hpp>

#include <Eigen/Core>

#define _USE_MATH_DEFINES
#include <math.h>

#include <limits>

namespace csCore {
namespace math {
  
TEMPLATE_TEST_CASE("Angles Conversion", "[math]", float32_t, float64_t) {
    using S = TestType;
    S const PRECISION = Eigen::NumTraits<S>::dummy_precision();

    REQUIRE_THAT(degToRad<S>(-450), Catch::Matchers::WithinAbs(-5.0 * M_PI / 2.0, PRECISION));
    REQUIRE_THAT(degToRad<S>(-360), Catch::Matchers::WithinAbs(-2.0 * M_PI, PRECISION));
    REQUIRE_THAT(degToRad<S>(-270), Catch::Matchers::WithinAbs(-3.0 * M_PI / 2.0, PRECISION));
    REQUIRE_THAT(degToRad<S>(-180), Catch::Matchers::WithinAbs(-M_PI, PRECISION));
    REQUIRE_THAT(degToRad<S>(-90), Catch::Matchers::WithinAbs(-M_PI / 2.0, PRECISION));
    REQUIRE_THAT(degToRad<S>(0), Catch::Matchers::WithinAbs(0, PRECISION));
    REQUIRE_THAT(degToRad<S>(90), Catch::Matchers::WithinAbs(M_PI / 2.0, PRECISION));
    REQUIRE_THAT(degToRad<S>(180), Catch::Matchers::WithinAbs(M_PI, PRECISION));
    REQUIRE_THAT(degToRad<S>(270), Catch::Matchers::WithinAbs(3.0 * M_PI / 2.0, PRECISION));
    REQUIRE_THAT(degToRad<S>(360), Catch::Matchers::WithinAbs(2.0 * M_PI, PRECISION));
    REQUIRE_THAT(degToRad<S>(450), Catch::Matchers::WithinAbs(5.0 * M_PI / 2.0, PRECISION));

    REQUIRE_THAT(radToDeg<S>(-5.0 * M_PI / 2.0), Catch::Matchers::WithinAbs(-450.0, PRECISION));
    REQUIRE_THAT(radToDeg<S>(-2.0 * M_PI), Catch::Matchers::WithinAbs(-360, PRECISION));
    REQUIRE_THAT(radToDeg<S>(-3.0 * M_PI / 2.0), Catch::Matchers::WithinAbs(-270, PRECISION));
    REQUIRE_THAT(radToDeg<S>(-M_PI), Catch::Matchers::WithinAbs(-180, PRECISION));
    REQUIRE_THAT(radToDeg<S>(-M_PI / 2.0), Catch::Matchers::WithinAbs(-90, PRECISION));
    REQUIRE_THAT(radToDeg<S>(0), Catch::Matchers::WithinAbs(0, PRECISION));
    REQUIRE_THAT(radToDeg<S>(M_PI / 2.0), Catch::Matchers::WithinAbs(90, PRECISION));
    REQUIRE_THAT(radToDeg<S>(M_PI), Catch::Matchers::WithinAbs(180, PRECISION));
    REQUIRE_THAT(radToDeg<S>(3.0 * M_PI / 2.0), Catch::Matchers::WithinAbs(270, PRECISION));
    REQUIRE_THAT(radToDeg<S>(2.0 * M_PI), Catch::Matchers::WithinAbs(360, PRECISION));
    REQUIRE_THAT(radToDeg<S>(5.0 * M_PI / 2.0), Catch::Matchers::WithinAbs(450.0, PRECISION));

}

TEMPLATE_TEST_CASE("Angles Conversion Contracts", "[math]", float32_t, float64_t) {
    using S = TestType;
    S const PRECISION = Eigen::NumTraits<S>::dummy_precision();

    radToDeg<S>(std::numeric_limits<S>::quiet_NaN());

}

}
}