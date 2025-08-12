#include <catch2/catch_all.hpp>
#include <csCore/core/Optional.hpp>

TEST_CASE("Swap value", "[swap.value]") {
  csCore::core::optional<int> o1 = 42;
  csCore::core::optional<int> o2 = 12;
  o1.swap(o2);
  CHECK(o1.value() == 12);
  CHECK(o2.value() == 42);
}

TEST_CASE("Swap value with null intialized", "[swap.value_nullopt]") {
  csCore::core::optional<int> o1 = 42;
  csCore::core::optional<int> o2 = tl::nullopt;
  o1.swap(o2);
  CHECK(!o1.has_value());
  CHECK(o2.value() == 42);
}

TEST_CASE("Swap null intialized with value", "[swap.nullopt_value]") {
  csCore::core::optional<int> o1 = tl::nullopt;
  csCore::core::optional<int> o2 = 42;
  o1.swap(o2);
  CHECK(o1.value() == 42);
  CHECK(!o2.has_value());
}
