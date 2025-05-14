#pragma once
#include <Eigen/Dense>
#include <csCore/csCoreAssert.hpp>
#include <csCore/eigenUtils/IsometryMatrix.hpp>
#include <csCore/eigenUtils/MatrixShape_impl.hpp>
#include <csCore/eigenUtils/RotationMatrix_impl.hpp>

namespace csCore {
namespace eigenUtils {

namespace _private {
template <Eigen::Index size, class Derived>
bool isValidIsometryMatrix(Eigen::MatrixBase<Derived> const& transf, typename Derived::Scalar precision) {
  using S = typename Derived::Scalar;

  CSCORE_ASSERT_DEBUG(precision >= static_cast<S>(0.0), "precision needs to be non negative");

  bool const validRot = _private::isValidRotationMatrix<size>(transf.template topLeftCorner<size, size>(), precision);

  if (!validRot) {
    return false;
  }

  // check last row below rotation part is all zeros
  for (Eigen::Index i = 0; i < size; i++) {
    // note: use equal on float, this elements should be exact and not
    // approximated
    if (transf(size, i) != 0) {
      return false;
    }
  }

  // check bottom right element is 1
  if (transf(size, size) != 1) {
    // note: use equal on float, this elements should be exact and not
    // approximated
    return false;
  }

  return true;
}

} // namespace _private

template <class Derived>
bool isValidIsometryMatrix2D(Eigen::MatrixBase<Derived> const& transf, typename Derived::Scalar precision) {
  using S = typename Derived::Scalar;
  using M33 = Eigen::Matrix<S, 3, 3>;
  CSCORE_ASSERT_DEBUG(csCore::eigenUtils::isSameSizeStaticOrDynamic(transf, M33()), "transf required to be 4x4");

  return _private::isValidIsometryMatrix<2>(transf, precision);
}

template <class S>
bool isValidIsometryMatrix2D(Eigen::Transform<S, 2, Eigen::Isometry> const& transf, S precision) {
  return isValidIsometryMatrix2D(transf.matrix(), precision);
}

template <class Derived>
bool isValidIsometryMatrix3D(Eigen::MatrixBase<Derived> const& transf, typename Derived::Scalar precision) {
  using S = typename Derived::Scalar;
  using M44 = Eigen::Matrix<S, 4, 4>;
  CSCORE_ASSERT_DEBUG(csCore::eigenUtils::isSameSizeStaticOrDynamic(transf, M44()), "transf required to be 4x4");

  return _private::isValidIsometryMatrix<3>(transf, precision);
}

/// @brief isValidIsometryMatrix3D accepting a Eigen::Transform
template <class S>
bool isValidIsometryMatrix3D(Eigen::Transform<S, 3, Eigen::Isometry> const& transf, S precision) {
  return isValidIsometryMatrix3D(transf.matrix(), precision);
}

} // namespace eigenUtils
} // namespace csCore
