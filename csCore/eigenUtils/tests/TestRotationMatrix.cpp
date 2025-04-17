#include <csCore/core/BasicTypes.hpp>
#include <csCore/csCoreAssert.hpp>
#include <csCore/eigenUtils/RotationMatrix_impl.hpp>
#include <csCore/testUtils/AssertCatch2.hpp>

namespace csCore {
namespace eigenUtils {

TEST_CASE("Rotation Matrix 2D", "[eigenUtils]") {
  using S = float32_t;

  // create a identity rotation matrix
  Eigen::Matrix<S, 2, 2> rI{Eigen::Matrix<S, 2, 2>::Identity()};
  REQUIRE(isValidRotationMatrix2D(rI));

  // change determinant sign
  rI(1, 1) = -1;
  REQUIRE(!isValidRotationMatrix2D(rI));

  // scale a bit
  rI(1, 1) = 1.05;
  REQUIRE(!isValidRotationMatrix2D(rI));

  // change precision to a big value
  REQUIRE(isValidRotationMatrix2D(rI, 10.0));

  // input parameters asserts
  if (csCore::Asserts::isAssertDebugEnabled()) {
    REQUIRE_ASSERT(isValidRotationMatrix2D(rI, -10.0));
    REQUIRE_ASSERT(isValidRotationMatrix2D(Eigen::MatrixX<S>(3, 3)));
  }
}

TEST_CASE("Rotation Matrix 3D", "[eigenUtils]") {
  using S = float32_t;

  // create a identity rotation matrix
  Eigen::Matrix<S, 3, 3> rI{Eigen::Matrix<S, 3, 3>::Identity()};
  REQUIRE(isValidRotationMatrix3D(rI));

  // change determinant sign
  rI(1, 1) = -1;
  REQUIRE(!isValidRotationMatrix3D(rI));

  // scale a bit
  rI(1, 1) = 1.05;
  REQUIRE(!isValidRotationMatrix3D(rI));

  // change precision to a big value
  REQUIRE(isValidRotationMatrix3D(rI, 10.0));

  // input parameters asserts
  if (csCore::Asserts::isAssertDebugEnabled()) {
    REQUIRE_ASSERT(isValidRotationMatrix3D(rI, -10.0));
    REQUIRE_ASSERT(isValidRotationMatrix3D(Eigen::MatrixX<S>(2, 2)));
  }
}

}  // namespace eigenUtils
}  // namespace csCore
