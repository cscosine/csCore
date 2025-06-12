#include <catch2/catch_all.hpp>
#include <csCore/pipes/fork.hpp>
#include <csCore/pipes/filter.hpp>
#include <csCore/pipes/override.hpp>
#include <csCore/pipes/transform.hpp>
#include <csCore/pipes/push_back.hpp>

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("fork dispatches an input to several destinations")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expected1 = numbers;
    std::vector<int> expected2 = numbers;
    std::vector<int> expected3 = numbers;

    std::vector<int> results1, results2, results3;
    
    std::copy(begin(numbers), end(numbers), csCore::pipes::fork(csCore::pipes::push_back(results1), csCore::pipes::push_back(results2), csCore::pipes::push_back(results3)));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
    REQUIRE(results3 == expected3);
}

TEST_CASE("fork can override existing results")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expected1 = numbers;
    std::vector<int> expected2 = numbers;
    std::vector<int> expected3 = numbers;
    
    std::vector<int> results1(numbers.size(), 0);
    std::vector<int> results2(numbers.size(), 0);
    std::vector<int> results3(numbers.size(), 0);
    
    std::copy(begin(numbers), end(numbers), csCore::pipes::fork(csCore::pipes::override(results1), csCore::pipes::override(results2), csCore::pipes::override(results3)));
    
    REQUIRE(results1 == expected1);
    REQUIRE(results2 == expected2);
    REQUIRE(results3 == expected3);
}

TEST_CASE("fork can send data to other pipes")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedMultiplesOf2 = {2, 4, 6, 8, 10};
    std::vector<int> expectedMultiplesOf3 = {3, 6, 9};
    std::vector<int> expectedMultiplesOf4 = {4, 8};

    std::vector<int> multiplesOf2, multiplesOf3, multiplesOf4;
    
    std::copy(begin(numbers), end(numbers), csCore::pipes::fork(csCore::pipes::filter([](int i){return i%2 == 0;}) >>= csCore::pipes::push_back(multiplesOf2),
                                                         csCore::pipes::filter([](int i){return i%3 == 0;}) >>= csCore::pipes::push_back(multiplesOf3),
                                                         csCore::pipes::filter([](int i){return i%4 == 0;}) >>= csCore::pipes::push_back(multiplesOf4)));
    
    REQUIRE(multiplesOf2 == expectedMultiplesOf2);
    REQUIRE(multiplesOf3 == expectedMultiplesOf3);
    REQUIRE(multiplesOf4 == expectedMultiplesOf4);
}

TEST_CASE("fork can be used as a tee")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedResults = {4, 8, 12, 16, 20};
    std::vector<int> expectedLog = {2, 4, 6, 8, 10};
    
    std::vector<int> results, log;
    
    std::copy(begin(numbers), end(numbers), csCore::pipes::filter([](int i){return i%2 == 0;})
                                        >>= csCore::pipes::fork(csCore::pipes::push_back(log),
                                                         csCore::pipes::transform([](int i){ return i*2; })
                                                         >>= csCore::pipes::push_back(results)));
    
    REQUIRE(results == expectedResults);
    REQUIRE(log == expectedLog);
}

TEST_CASE("fork's iterator category should be std::output_iterator_tag")
{
    std::vector<int> output;
    static_assert(std::is_same<decltype(csCore::pipes::fork(csCore::pipes::push_back(output), csCore::pipes::push_back(output), csCore::pipes::push_back(output)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("fork operator=")
{
    std::vector<int> results1, results2, results3, results4;
    auto fork1 = csCore::pipes::fork(csCore::pipes::push_back(results1), csCore::pipes::push_back(results2));
    auto fork2 = csCore::pipes::fork(csCore::pipes::push_back(results1), csCore::pipes::push_back(results2));

    fork2 = fork1;
    csCore::pipes::send(0, fork2);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}
