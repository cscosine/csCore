#include <catch2/catch_all.hpp>
#include <csCore/core/Optional.hpp>
#include <tuple>
#include <utility>

TEST_CASE("Emplace", "[emplace]") {
  csCore::core::optional<std::pair<std::pair<int, int>, std::pair<double, double>>> i;
  i.emplace(std::piecewise_construct, std::make_tuple(0, 2), std::make_tuple(3, 4));
  REQUIRE(i->first.first == 0);
  REQUIRE(i->first.second == 2);
  REQUIRE(i->second.first == 3);
  REQUIRE(i->second.second == 4);
}

struct A {
  A() {
    throw std::exception();
  }
};

TEST_CASE("Emplace with exception thrown", "[emplace]") {
  csCore::core::optional<A> a;
  REQUIRE_THROWS(a.emplace());
}
