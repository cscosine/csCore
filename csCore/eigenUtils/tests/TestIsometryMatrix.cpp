#include <csCore/core/BasicTypes.hpp>
#include <csCore/csCoreAssert.hpp>
#include <csCore/eigenUtils/IsometryMatrix_impl.hpp>
#include <csCore/testUtils/AssertCatch2.hpp>

namespace csCore {
namespace eigenUtils {

TEST_CASE("Isometry Matrix 2D", "[eigenUtils]") {
  using S = float32_t;

  // create a identity isometry matrix
  Eigen::Matrix<S, 3, 3> rI{Eigen::Matrix<S, 3, 3>::Identity()};
  REQUIRE(isValidIsometryMatrix2D(rI));

  // change determinant sign
  rI(1, 1) = -1;
  REQUIRE(!isValidIsometryMatrix2D(rI));

  // scale a bit
  rI(1, 1) = 1.05;
  REQUIRE(!isValidIsometryMatrix2D(rI));

  // change precision to a big value
  REQUIRE(isValidIsometryMatrix2D(rI, 10.0));

  // back to good values
  rI(1, 1) = 1.0;
  REQUIRE(isValidIsometryMatrix2D(rI));

  // perturb last row
  rI(2, 0) = 0.005;
  REQUIRE(!isValidIsometryMatrix2D(rI));

  // back to good values
  rI(2, 0) = 0;
  REQUIRE(isValidIsometryMatrix2D(rI));

  // perturb bottom right element
  rI(2, 2) = 1.005;
  REQUIRE(!isValidIsometryMatrix2D(rI));

  // input parameters asserts
  if (csCore::Asserts::isAssertDebugEnabled()) {
    REQUIRE_ASSERT(isValidIsometryMatrix2D(rI, -10.0));
    REQUIRE_ASSERT(isValidIsometryMatrix2D(Eigen::MatrixX<S>(4, 4)));
  }

  // isometry
  REQUIRE(isValidIsometryMatrix2D(Eigen::Transform<S, 2, Eigen::Isometry>::Identity()));
}

TEST_CASE("Isometry Matrix 3D", "[eigenUtils]") {
  using S = float32_t;

  // create a identity isometry matrix
  Eigen::Matrix<S, 4, 4> rI{Eigen::Matrix<S, 4, 4>::Identity()};
  REQUIRE(isValidIsometryMatrix3D(rI));

  // change determinant sign
  rI(1, 1) = -1;
  REQUIRE(!isValidIsometryMatrix3D(rI));

  // scale a bit
  rI(1, 1) = 1.05;
  REQUIRE(!isValidIsometryMatrix3D(rI));

  // change precision to a big value
  REQUIRE(isValidIsometryMatrix3D(rI, 10.0));

  // back to good values
  rI(1, 1) = 1.0;
  REQUIRE(isValidIsometryMatrix3D(rI));

  // perturb last row
  rI(2, 0) = 0.005;
  REQUIRE(!isValidIsometryMatrix3D(rI));

  // back to good values
  rI(2, 0) = 0;
  REQUIRE(isValidIsometryMatrix3D(rI));

  // perturb bottom right element
  rI(3, 3) = 1.005;
  REQUIRE(!isValidIsometryMatrix3D(rI));

  // input parameters asserts
  if (csCore::Asserts::isAssertDebugEnabled()) {
    REQUIRE_ASSERT(isValidIsometryMatrix3D(rI, -10.0));
    REQUIRE_ASSERT(isValidIsometryMatrix3D(Eigen::MatrixX<S>(3, 3)));
  }

  // isometry
  REQUIRE(isValidIsometryMatrix3D(Eigen::Transform<S, 3, Eigen::Isometry>::Identity()));
}

} // namespace eigenUtils
} // namespace csCore
