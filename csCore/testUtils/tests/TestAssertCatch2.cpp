#include <csCore/testUtils/AssertCatch2.hpp>
#include <stdexcept>

namespace {

void noAssertion() {}

void customHandler(::libassert::assertion_info const &) {}

} // namespace

TEST_CASE("REQUIRE_NO_ASSERT restores the failure handler", "[testUtils]") {
  ::libassert::handler_ptr const before = ::libassert::get_failure_handler();
  ::libassert::set_failure_handler(&customHandler);
  REQUIRE_NO_ASSERT(noAssertion());
  REQUIRE(::libassert::get_failure_handler() == &customHandler);
  ::libassert::set_failure_handler(before);
}

TEST_CASE("FailureHandlerGuard restores the failure handler when left by an exception", "[testUtils]") {
  // the path of a failing REQUIRE_NO_ASSERT: Catch2's FAIL throws, a failing test cannot be tested directly
  ::libassert::handler_ptr const before = ::libassert::get_failure_handler();
  ::libassert::set_failure_handler(&customHandler);
  try {
    ::csCore::testUtils::_private::FailureHandlerGuard const guard{&::csCore::testUtils::_private::throwAssertion};
    REQUIRE(::libassert::get_failure_handler() == &::csCore::testUtils::_private::throwAssertion);
    throw std::runtime_error("leaving the scope");
  } catch (std::runtime_error const &) {
  }
  REQUIRE(::libassert::get_failure_handler() == &customHandler);
  ::libassert::set_failure_handler(before);
}
