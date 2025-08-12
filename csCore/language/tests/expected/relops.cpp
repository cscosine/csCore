#include <catch2/catch_all.hpp>
#include <csCore/language/Expected.hpp>

TEST_CASE("Relational operators", "[relops]") {
  csCore::language::expected<int, int> o1 = 42;
  csCore::language::expected<int, int> o2{tl::unexpect, 0};
  const csCore::language::expected<int, int> o3 = 42;

  REQUIRE(o1 == o1);
  REQUIRE(o1 != o2);
  REQUIRE(o1 == o3);
  REQUIRE(o3 == o3);

  csCore::language::expected<void, int> o6;

  REQUIRE(o6 == o6);
}
