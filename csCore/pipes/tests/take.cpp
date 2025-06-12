#include <catch2/catch_all.hpp>
#include <csCore/pipes/pipes.hpp>

#include <vector>

TEST_CASE("take takes the first N elements coming from a range") {
  auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto const expected = std::vector<int>{1, 2, 3, 4, 5, 6};

  auto result = std::vector<int>{};

  input >>= csCore::pipes::take(6) >>= csCore::pipes::push_back(result);

  REQUIRE(result == expected);
}

TEST_CASE("take takes the first N elements coming from another pipe") {
  auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto const expected = std::vector<int>{2, 4};

  auto result = std::vector<int>{};

  input >>= csCore::pipes::filter([](int i) {
    return i % 2 == 0;
  }) >>= csCore::pipes::take(2) >>= csCore::pipes::push_back(result);

  REQUIRE(result == expected);
}

TEST_CASE("take(N) takes all the elements if there are less than N") {
  auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto const expected = std::vector<int>{2, 4, 6, 8, 10};

  auto result = std::vector<int>{};

  input >>= csCore::pipes::filter([](int i) {
    return i % 2 == 0;
  }) >>= csCore::pipes::take(6) >>= csCore::pipes::push_back(result);

  REQUIRE(result == expected);
}

TEST_CASE("take handles empty input") {
  auto const input = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto const expected = std::vector<int>{};

  auto result = std::vector<int>{};

  input >>= csCore::pipes::filter([](int) {
    return false;
  }) >>= csCore::pipes::take(6) >>= csCore::pipes::push_back(result);

  REQUIRE(result == expected);
}

TEST_CASE("takes can handle multiple values") {
  auto const input1 = std::vector<int>{1, 2, 3, 4, 5};
  auto const input2 = std::vector<int>{10, 20, 30, 40, 50};

  auto const expected = std::vector<int>{11, 22, 33};

  auto result = std::vector<int>{};

  csCore::pipes::mux(input1, input2) >>= csCore::pipes::take(3) >>= csCore::pipes::transform([](int a, int b) {
    return a + b;
  }) >>= csCore::pipes::push_back(result);

  REQUIRE(result == expected);
}

TEST_CASE("take operator=") {
  std::vector<int> results1, results2;
  auto pipeline1 = csCore::pipes::take(1) >>= csCore::pipes::push_back(results1);
  auto pipeline2 = csCore::pipes::take(2) >>= csCore::pipes::push_back(results2);

  pipeline2 = pipeline1;
  send(1, pipeline2);
  send(2, pipeline2);
  send(3, pipeline2);
  REQUIRE(results1.size() == 1);
  REQUIRE(results2.size() == 0);
}
