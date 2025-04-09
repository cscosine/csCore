#pragma once
#include <csCore/eigenUtils/RotationMatrix.hpp>
#include <csCore/csCoreAssert.hpp>

#include <csCore/eigenUtils/MatrixShape_impl.hpp>

#include <Eigen/Dense>

namespace csCore {
    namespace eigenUtils {

        template <class Derived> 
        bool isValidRotationMatrix2D(Eigen::MatrixBase<Derived> const& rot, typename Derived::Scalar precision)
        {
          using S = typename Derived::Scalar;
      
          using M22 = Eigen::Matrix<S, 2, 2>;
          CSCORE_CONTRACT_STANDARD(precision >= static_cast<S>(0.0), "precision needs to be non negative");
          CSCORE_CONTRACT_STANDARD(csCore::eigenUtils::isSameSizeStaticOrDynamic(rot, M22()), "rot required to be 2x2");
      
          S const det{rot.determinant()};
          if(std::abs(det - static_cast<S>(1.0)) >= precision) {
            return false;
          }
          
          if(!(rot * rot.transpose() - Eigen::Matrix2<S>::Identity()).isZero(precision)) {
            return false;
          }
      
          if(!(rot.transpose() * rot - Eigen::Matrix2<S>::Identity()).isZero(precision)) {
            return false;
          }
      
          return true;
        }
      
    }
}

