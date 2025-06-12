#include <catch2/catch_all.hpp>
#include <csCore/pipes/operator.hpp>
#include <csCore/pipes/push_back.hpp>
#include <csCore/pipes/tap.hpp>
#include <csCore/pipes/transform.hpp>

#include <algorithm>
#include <vector>
#include <list>

typedef unsigned int uint;

TEST_CASE("tap")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7 ,8, 9, 10};
    std::vector<int> expected = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    std::vector<int> results;

    input >>= csCore::pipes::transform([](auto const& x) { return x * 2; })
          >>= csCore::pipes::tap([&expected, i = uint{0}](auto const& x) mutable { REQUIRE(x == expected[i++]); })
          >>= csCore::pipes::push_back(results);
    
    REQUIRE(results == expected);
}

TEST_CASE("tap operator=")
{
    using range_type = std::list<int>;

    class FrontInserter
    {
    public:
        FrontInserter(range_type& range) : range_(range) {}

        void operator()(range_type::value_type value) const 
        {
            range_.push_front(value);
        } 
    private:
        range_type& range_;
    };

    range_type results1, results2, results3, results4;
    
    auto tap1 = csCore::pipes::tap(FrontInserter(results1)) >>= csCore::pipes::push_back(results2);
    auto tap2 = csCore::pipes::tap(FrontInserter(results3)) >>= csCore::pipes::push_back(results4);
    
    tap2 = tap1;
    csCore::pipes::send(0, tap2);
    
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 1);
    REQUIRE(results3.size() == 0);
    REQUIRE(results4.size() == 0);
}