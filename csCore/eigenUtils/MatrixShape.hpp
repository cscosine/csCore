#pragma once
#include <Eigen/Core>

namespace csCore {
namespace eigenUtils {

template <class Derived>
bool checkSizeEqual(Eigen::EigenBase<Derived> const& matrix, Eigen::Index rows,
                    Eigen::Index cols);

template <class DerivedA, class DerivedB>
bool isSameSizeStaticOrDynamic(Eigen::EigenBase<DerivedA> const& matrixA,
                               Eigen::EigenBase<DerivedB> const& matrixB);

template <Eigen::Index rows, Eigen::Index cols, class DerivedA>
bool isSameSizeStaticOrDynamic(Eigen::EigenBase<DerivedA> const& matrixA);

}  // namespace eigenUtils
}  // namespace csCore
