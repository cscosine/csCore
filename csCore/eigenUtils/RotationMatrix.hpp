#pragma once
#include <Eigen/Core>

namespace csCore {
namespace eigenUtils {

/// @brief check if a matrix is a valid 2D rotation matrix, i.e., det(R) = 1,
/// R*R^T = I and R^T*R = I, withing a tolerance
/// @param rot the rotation matrix
/// @param precision the tolerance
/// @throw if rot is not a 2x2 matrix or precision < 0 (DEBUG only)
template <class Derived>
bool isValidRotationMatrix2D(
    Eigen::MatrixBase<Derived> const &rot,
    typename Derived::Scalar precision = Eigen::NumTraits<typename Derived::Scalar>::dummy_precision());

/// @brief check if a matrix is a valid 3D rotation matrix, i.e., det(R) = 1,
/// R*R^T = I and R^T*R = I, withing a tolerance
/// @param rot the rotation matrix
/// @param precision the tolerance
/// @throw if rot is not a 3x3 matrix or precision < 0 (DEBUG only)
template <class Derived>
bool isValidRotationMatrix3D(
    Eigen::MatrixBase<Derived> const &rot,
    typename Derived::Scalar precision = Eigen::NumTraits<typename Derived::Scalar>::dummy_precision());

} // namespace eigenUtils
} // namespace csCore
