#include <csCore/core/BasicTypes.hpp>
#include <csCore/csCoreAssert.hpp>
#include <csCore/math/Angles.hpp>
#include <csCore/testUtils/AssertCatch2.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include <limits>

namespace csCore {
namespace math {

TEMPLATE_TEST_CASE("Angles Conversion", "[math]", float32_t, float64_t) {
  using S = TestType;
  S const PRECISION = std::numeric_limits<S>::epsilon() * 10.0;

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

  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(90)), Catch::Matchers::WithinAbs(degToRad<S>(90), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(179)), Catch::Matchers::WithinAbs(degToRad<S>(179), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(181)), Catch::Matchers::WithinAbs(degToRad<S>(-179), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(270)), Catch::Matchers::WithinAbs(degToRad<S>(-90), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(359)), Catch::Matchers::WithinAbs(degToRad<S>(-1), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(539)), Catch::Matchers::WithinAbs(degToRad<S>(179), PRECISION));

  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(-90)), Catch::Matchers::WithinAbs(degToRad<S>(-90), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(-179)), Catch::Matchers::WithinAbs(degToRad<S>(-179), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(-181)), Catch::Matchers::WithinAbs(degToRad<S>(179), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(-270)), Catch::Matchers::WithinAbs(degToRad<S>(90), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(-359)), Catch::Matchers::WithinAbs(degToRad<S>(1), PRECISION));
  REQUIRE_THAT(wrapToPi<S>(degToRad<S>(-539)), Catch::Matchers::WithinAbs(degToRad<S>(-179), PRECISION));

  REQUIRE_THAT(angleDistance<S>(degToRad<S>(0), degToRad<S>(30)),
               Catch::Matchers::WithinAbs(degToRad<S>(30), PRECISION));
  REQUIRE_THAT(angleDistance<S>(degToRad<S>(0), degToRad<S>(-30)),
               Catch::Matchers::WithinAbs(degToRad<S>(-30), PRECISION));

  REQUIRE_THAT(angleDistance<S>(degToRad<S>(30), degToRad<S>(30)),
               Catch::Matchers::WithinAbs(degToRad<S>(0), PRECISION));
  REQUIRE_THAT(angleDistance<S>(degToRad<S>(30), degToRad<S>(-30)),
               Catch::Matchers::WithinAbs(degToRad<S>(-60), PRECISION));
  REQUIRE_THAT(angleDistance<S>(degToRad<S>(30), degToRad<S>(60)),
               Catch::Matchers::WithinAbs(degToRad<S>(30), PRECISION));
  REQUIRE_THAT(angleDistance<S>(degToRad<S>(60), degToRad<S>(30)),
               Catch::Matchers::WithinAbs(degToRad<S>(-30), PRECISION));

  REQUIRE_THAT(angleDistance<S>(degToRad<S>(30), degToRad<S>(360 + 30)),
               Catch::Matchers::WithinAbs(degToRad<S>(0), PRECISION));
  REQUIRE_THAT(angleDistance<S>(degToRad<S>(30 + 720), degToRad<S>(360 + 60)),
               Catch::Matchers::WithinAbs(degToRad<S>(30), PRECISION));
}

TEMPLATE_TEST_CASE("Angles Conversion Safe Math", "[math]", float32_t, float64_t) {
  if (csCore::Asserts::isAssertSafeMathEnabled()) {
    using S = TestType;
    REQUIRE_ASSERT(radToDeg<S>(std::numeric_limits<S>::quiet_NaN()));

    REQUIRE_ASSERT(degToRad<S>(std::numeric_limits<S>::quiet_NaN()));

    REQUIRE_ASSERT(wrapToPi<S>(std::numeric_limits<S>::quiet_NaN()));

    REQUIRE_ASSERT(angleDistance<S>(std::numeric_limits<S>::quiet_NaN(), 1.0));
    REQUIRE_ASSERT(angleDistance<S>(1.0, std::numeric_limits<S>::quiet_NaN()));
  }

  // radToDeg<int>(90); //does not compile
  // radToDeg(90); //does not compile
}

} // namespace math
} // namespace csCore
