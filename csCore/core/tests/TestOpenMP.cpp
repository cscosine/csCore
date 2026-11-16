#include <csCore/core/OpenMP.hpp>

#include <catch2/catch_test_macros.hpp>

#include <vector>

namespace csCore {
namespace core {

TEST_CASE("CS_OMP: the same result with and without OpenMP", "[core]") {
  std::vector<int> values(1000);
  for (int i = 0; i < static_cast<int>(values.size()); ++i)
    values[static_cast<std::size_t>(i)] = i;

  long long sum = 0;
  int const size = static_cast<int>(values.size());
  CS_OMP(parallel for schedule(static) reduction(+ : sum))
  for (int i = 0; i < size; ++i)
    sum += values[static_cast<std::size_t>(i)];

  REQUIRE(sum == 999LL * 1000LL / 2LL);
}

TEST_CASE("ompMaxThreads, ompThreadNum", "[core]") {
#ifdef _OPENMP
  REQUIRE(ompMaxThreads() >= 1);
#else
  REQUIRE(ompMaxThreads() == 1);
#endif
  REQUIRE(ompThreadNum() == 0); // outside a parallel region
}

} // namespace core
} // namespace csCore
