#include <csCore/core/BasicTypes.hpp>
#include <csCore/csCoreAssert.hpp>
#include <csCore/eigenUtils/MatrixShape_impl.hpp>
#include <csCore/testUtils/AssertCatch2.hpp>

namespace csCore {
namespace eigenUtils {

TEST_CASE("Matrix Shape", "[eigenUtils]") {
  Eigen::Matrix<int, 3, 2> m32;

  REQUIRE(checkSizeEqual(m32, 3, 2) == true);
  REQUIRE(checkSizeEqual(m32, 3, 3) == false);
  REQUIRE(checkSizeEqual(m32, 2, 3) == false);
  REQUIRE(checkSizeEqual(m32, 4, 3) == false);

  REQUIRE(isSameSizeStaticOrDynamic(m32, Eigen::MatrixXd(3, 2)) == true);
  REQUIRE(isSameSizeStaticOrDynamic(m32, Eigen::MatrixXd(3, 3)) == false);
  REQUIRE(isSameSizeStaticOrDynamic(m32, Eigen::MatrixXd(2, 3)) == false);
  REQUIRE(isSameSizeStaticOrDynamic(m32, Eigen::MatrixXd(4, 3)) == false);

  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::MatrixXd(3, 2)) == true);
  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::MatrixXd(3, 3)) == false);
  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::MatrixXd(2, 3)) == false);
  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::MatrixXd(4, 3)) == false);

  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::Matrix<float32_t, 3, 2>()) == true);
  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::Matrix<float32_t, 3, 3>()) == false);
  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::Matrix<float32_t, 2, 3>()) == false);
  REQUIRE(isSameSizeStaticOrDynamic(Eigen::MatrixXd(3, 2),
                                    Eigen::Matrix<float32_t, 4, 3>()) == false);

  REQUIRE(isSameSizeStaticOrDynamic<3, 2>(Eigen::MatrixXd(3, 2)) == true);
  REQUIRE(isSameSizeStaticOrDynamic<3, 3>(Eigen::MatrixXd(3, 2)) == false);
  REQUIRE(isSameSizeStaticOrDynamic<2, 3>(Eigen::MatrixXd(3, 2)) == false);
  REQUIRE(isSameSizeStaticOrDynamic<4, 3>(Eigen::MatrixXd(3, 2)) == false);

  REQUIRE(isSameSizeStaticOrDynamic<3, 2>(m32) == true);
  // REQUIRE(isSameSizeStaticOrDynamic<3,3>(m32)== false); // does not compile
  // REQUIRE(isSameSizeStaticOrDynamic<2,3>(m32)== false); // does not compile
  // REQUIRE(isSameSizeStaticOrDynamic<4,3>(m32)== false); // does not compile
}

}  // namespace eigenUtils
}  // namespace csCore
