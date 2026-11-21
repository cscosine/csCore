#pragma once
#include <catch2/catch_all.hpp>
#include <libassert/assert-catch2.hpp>
#include <libassert/assert.hpp>

#define REQUIRE_NO_ASSERT(expr)                                                                                        \
  do {                                                                                                                 \
    auto handler = ::libassert::get_failure_handler();                                                                 \
    ::libassert::set_failure_handler([](const ::libassert::assertion_info &info) { throw info; });                     \
    bool did_assert = false;                                                                                           \
    try {                                                                                                              \
      (expr);                                                                                                          \
    } catch (const ::libassert::assertion_info &info) {                                                                \
      did_assert = true;                                                                                               \
      FAIL("Expected no assertion failure from " #expr " however it happened");                                        \
    }                                                                                                                  \
    if (!did_assert) {                                                                                                 \
      SUCCEED();                                                                                                       \
    }                                                                                                                  \
    ::libassert::set_failure_handler(handler);                                                                         \
  } while (false)
