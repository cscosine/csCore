#pragma once

// OpenMP when the code is compiled with it (_OPENMP: csCMake cs_target_use_openmp), serial otherwise (e.g. Apple
// Clang, which has no OpenMP runtime): no #pragma omp nor OpenMP call outside these, so that a build without OpenMP
// has neither unknown pragmas nor missing symbols.
//
//   CS_OMP(parallel for num_threads(n) schedule(static) reduction(+ : sum))
//   for (int i = 0; i < size; ++i) { ... }
//
// CS_OMP(<directive>): #pragma omp <directive> with OpenMP, nothing without.

#ifdef _OPENMP
#include <omp.h>

#define CS_OMP_STRINGIFY_(...) #__VA_ARGS__
#if defined(_MSC_VER) && !defined(__clang__)
#define CS_OMP(...) __pragma(omp __VA_ARGS__)
#else
#define CS_OMP(...) _Pragma(CS_OMP_STRINGIFY_(omp __VA_ARGS__))
#endif
#else
#define CS_OMP(...)
#endif

namespace csCore {
namespace core {

// the number of threads a parallel region would use (omp_get_max_threads); 1 without OpenMP
inline int ompMaxThreads() noexcept {
#ifdef _OPENMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}

// the number of the calling thread in its parallel region (omp_get_thread_num); 0 without OpenMP
inline int ompThreadNum() noexcept {
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}

} // namespace core
} // namespace csCore
