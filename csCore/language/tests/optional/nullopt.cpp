#include <catch2/catch_all.hpp>
#include <csCore/language/Optional.hpp>

TEST_CASE("Nullopt", "[nullopt]") {
  csCore::language::optional<int> o1 = tl::nullopt;
  csCore::language::optional<int> o2{tl::nullopt};
  csCore::language::optional<int> o3(tl::nullopt);
  csCore::language::optional<int> o4 = {tl::nullopt};

  REQUIRE(!o1);
  REQUIRE(!o2);
  REQUIRE(!o3);
  REQUIRE(!o4);

  REQUIRE(!std::is_default_constructible<tl::nullopt_t>::value);
}
