#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace csCore {
namespace eigenUtils {

/// @brief check if a matrix is a valid 2D isometry matrix, i.e., [R, t; 001];
/// @param transf the isometry matrix
/// @param precision the tolerance
/// @throw if transf is not a 3x3 matrix or precision < 0 (DEBUG only)
template <class Derived>
bool isValidIsometryMatrix2D(
    Eigen::MatrixBase<Derived> const& transf,
    typename Derived::Scalar precision =
        Eigen::NumTraits<typename Derived::Scalar>::dummy_precision());

/// @brief isValidIsometryMatrix2D accepting a Eigen::Transform
template <class S>
bool isValidIsometryMatrix2D(
    Eigen::Transform<S, 2, Eigen::Isometry> const& transf,
    S precision = Eigen::NumTraits<S>::dummy_precision());

/// @brief check if a matrix is a valid 3D isometry matrix, i.e., [R, t; 0001];
/// @param transf the isometry matrix
/// @param precision the tolerance
/// @throw if transf is not a 4x4 matrix or precision < 0 (DEBUG only)
template <class Derived>
bool isValidIsometryMatrix3D(
    Eigen::MatrixBase<Derived> const& transf,
    typename Derived::Scalar precision =
        Eigen::NumTraits<typename Derived::Scalar>::dummy_precision());

/// @brief isValidIsometryMatrix3D accepting a Eigen::Transform
template <class S>
bool isValidIsometryMatrix3D(
    Eigen::Transform<S, 3, Eigen::Isometry> const& transf,
    S precision = Eigen::NumTraits<S>::dummy_precision());

}  // namespace eigenUtils
}  // namespace csCore
