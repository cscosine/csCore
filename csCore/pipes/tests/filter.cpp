#include <algorithm>

#include <catch2/catch_all.hpp>
#include <csCore/pipes/filter.hpp>
#include <csCore/pipes/override.hpp>
#include <csCore/pipes/push_back.hpp>

TEST_CASE("filter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = csCore::pipes::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    std::vector<int> results;
    std::copy(begin(input), end(input), ifIsEven >>= csCore::pipes::push_back(results));
    
    REQUIRE(results == expected);
}

TEST_CASE("filter can override existing results")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = csCore::pipes::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10, 0, 0, 0, 0, 0};
    
    std::vector<int> results = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::copy(begin(input), end(input), ifIsEven >>= csCore::pipes::override(results));
    
    REQUIRE(results == expected);
}

TEST_CASE("filter's iterator category should be std::output_iterator_tag")
{
    auto const isEven = csCore::pipes::filter([](int i) { return i % 2 == 0; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(isEven >>= csCore::pipes::push_back(output))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("filter operator=")
{
    std::vector<int> results1, results2;
    auto predicate = [](int i){ return i > 0; };
    auto pipeline1 = csCore::pipes::filter(predicate) >>= csCore::pipes::push_back(results1);
    auto pipeline2 = csCore::pipes::filter(predicate) >>= csCore::pipes::push_back(results2);
    
    pipeline2 = pipeline1;
    send(1, pipeline2);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}

TEST_CASE("filter doesn't move values into the predicate")
{
    struct Moveable
    {
        int i_;
        explicit Moveable(int i) : i_{i}{}
        Moveable(Moveable&& other) : i_(other.i_) { other.i_ = 0; }
        Moveable(Moveable const&) = default;
        bool operator==(Moveable other) const { return i_ == other.i_; }
    };
    
    auto input = std::vector<Moveable>{Moveable{0}, Moveable{1}, Moveable{2}, Moveable{3}};
    auto expectedInput = std::vector<Moveable>{Moveable{0}, Moveable{1}, Moveable{0}, Moveable{3}};
    auto expectedResult = std::vector<Moveable>{Moveable{0}, Moveable{2}};
    
    auto result = std::vector<Moveable>{};
    
    std::copy(std::make_move_iterator(begin(input)), std::make_move_iterator(end(input)),
              csCore::pipes::filter([](Moveable m){ return m.i_ % 2 == 0; })
          >>= csCore::pipes::push_back(result));
    
    REQUIRE(result == expectedResult);
    REQUIRE(input == expectedInput);
}
