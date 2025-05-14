#pragma once
#include <Eigen/Core>
#include <csCore/eigenUtils/MatrixShape.hpp>

namespace csCore {
namespace eigenUtils {

template <class Derived>
bool checkSizeEqual(Eigen::EigenBase<Derived> const& matrix, Eigen::Index rows, Eigen::Index cols) {
  return (matrix.rows() == rows) && (matrix.cols() == cols);
}

template <class DerivedA, class DerivedB>
bool isSameSizeStaticOrDynamic(Eigen::EigenBase<DerivedA> const& matrixA, Eigen::EigenBase<DerivedB> const& matrixB) {
  EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(DerivedA, DerivedB);
  return checkSizeEqual(matrixA, matrixB.rows(), matrixB.cols());
}

template <Eigen::Index rows, Eigen::Index cols, class DerivedA>
bool isSameSizeStaticOrDynamic(Eigen::EigenBase<DerivedA> const& matrixA) {
  using MB = Eigen::Matrix<int, rows, cols>;
  EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(DerivedA, MB);
  return checkSizeEqual(matrixA, rows, cols);
}

} // namespace eigenUtils
} // namespace csCore
