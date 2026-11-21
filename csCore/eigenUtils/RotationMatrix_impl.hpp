#pragma once
#include <Eigen/Dense>
#include <csCore/csCoreAssert.hpp>
#include <csCore/eigenUtils/MatrixShape_impl.hpp>
#include <csCore/eigenUtils/RotationMatrix.hpp>

namespace csCore {
namespace eigenUtils {

namespace _private {
template <Eigen::Index size, class Derived>
bool isValidRotationMatrix(Eigen::MatrixBase<Derived> const &rot, typename Derived::Scalar precision) {
  using S = typename Derived::Scalar;

  CSCORE_ASSERT_DEBUG(precision >= static_cast<S>(0.0), "precision needs to be non negative");

  S const det{rot.determinant()};

  if (std::abs(det - static_cast<S>(1.0)) >= precision) {
    return false;
  }

  if (!(rot * rot.transpose() - Eigen::Matrix<S, size, size>::Identity()).isZero(precision)) {
    return false;
  }

  if (!(rot.transpose() * rot - Eigen::Matrix<S, size, size>::Identity()).isZero(precision)) {
    return false;
  }
  return true;
}

} // namespace _private

template <class Derived>
bool isValidRotationMatrix2D(Eigen::MatrixBase<Derived> const &rot, typename Derived::Scalar precision) {
  CSCORE_ASSERT_DEBUG(
      csCore::eigenUtils::isSameSizeStaticOrDynamic(rot, (Eigen::Matrix<typename Derived::Scalar, 2, 2>())),
      "rot required to be 2x2");

  return _private::isValidRotationMatrix<2>(rot, precision);
}

template <class Derived>
bool isValidRotationMatrix3D(Eigen::MatrixBase<Derived> const &rot, typename Derived::Scalar precision) {
  CSCORE_ASSERT_DEBUG(
      csCore::eigenUtils::isSameSizeStaticOrDynamic(rot, (Eigen::Matrix<typename Derived::Scalar, 3, 3>())),
      "rot required to be 2x2");

  return _private::isValidRotationMatrix<3>(rot, precision);
}

} // namespace eigenUtils
} // namespace csCore
