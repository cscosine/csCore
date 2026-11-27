#pragma once
#include <catch2/catch_all.hpp>
#include <libassert/assert-catch2.hpp>
#include <libassert/assert.hpp>

namespace csCore::testUtils::_private {

/// @brief the failure handler of REQUIRE_NO_ASSERT: the assertion is thrown, so the macro can catch it
[[noreturn]] inline void throwAssertion(::libassert::assertion_info const &info) { throw info; }

/// @brief sets a libassert failure handler and restores the previous one at the end of its scope, also when the scope
/// is left by an exception (Catch2's FAIL throws)
class FailureHandlerGuard {
public:
  explicit FailureHandlerGuard(::libassert::handler_ptr const handler) : saved_{::libassert::get_failure_handler()} {
    ::libassert::set_failure_handler(handler);
  }
  ~FailureHandlerGuard() { ::libassert::set_failure_handler(saved_); }
  FailureHandlerGuard(FailureHandlerGuard const &) = delete;
  FailureHandlerGuard &operator=(FailureHandlerGuard const &) = delete;
  FailureHandlerGuard(FailureHandlerGuard &&) = delete;
  FailureHandlerGuard &operator=(FailureHandlerGuard &&) = delete;

private:
  ::libassert::handler_ptr saved_;
};

} // namespace csCore::testUtils::_private

/// @brief requires that expr does not fail a libassert assertion; on a failure, the message says which one. The
/// failure handler in place before is restored in every case
#define REQUIRE_NO_ASSERT(expr)                                                                                        \
  do {                                                                                                                 \
    ::csCore::testUtils::_private::FailureHandlerGuard const csCoreHandlerGuard{                                       \
        &::csCore::testUtils::_private::throwAssertion};                                                               \
    try {                                                                                                              \
      (expr);                                                                                                          \
      SUCCEED();                                                                                                       \
    } catch (const ::libassert::assertion_info &csCoreAssertion) {                                                     \
      FAIL("Expected no assertion failure from " #expr ", however it happened:\n"                                      \
           << csCoreAssertion.header(0, ::libassert::color_scheme::blank));                                            \
    }                                                                                                                  \
  } while (false)
