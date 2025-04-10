#include <csCore/testUtils/AssertCatch2.hpp>

#include <csCore/core/BasicTypes.hpp>

#include <csCore/eigenUtils/RotationMatrix_impl.hpp>
#include <csCore/csCoreAssert.hpp>

namespace csCore {
namespace eigenUtils {
 
  TEST_CASE("Rotation Matrix", "[eigenUtils]") {
    using S = float32_t;

    // create a identity rotation matrix
    Eigen::Matrix<S,2,2> rI{ Eigen::Matrix<S,2,2>::Identity()};
    REQUIRE(isValidRotationMatrix2D(rI));

    // change determinant sign
    rI(1,1) = -1;
    REQUIRE(!isValidRotationMatrix2D(rI));

    // scale a bit
    rI(1,1) = 1.05;
    REQUIRE(!isValidRotationMatrix2D(rI));

    // change precision to a big value
    REQUIRE(isValidRotationMatrix2D(rI, 10.0));

    // input parameters asserts
    if(csCore::Asserts::isAssertDebugEnabled()) {
      REQUIRE_ASSERT(isValidRotationMatrix2D(rI, -10.0));
      REQUIRE_ASSERT(isValidRotationMatrix2D(Eigen::MatrixX<S>(3,3)));
    }
  }

}
}