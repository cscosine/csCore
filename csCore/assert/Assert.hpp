#pragma once
#include <csCore/csCore.hpp>

#include <libassert/assert.hpp>

// just a overload of libassert defines
#define CS_DEBUG_ASSERT(...) DEBUG_ASSERT(__VA_ARGS__)
#define CS_ASSERT(...) ASSERT(__VA_ARGS__)
#define CS_ASSUME(...) ASSUME(__VA_ARGS__)
#define CS_PANIC(...) PANIC(__VA_ARGS__)
#define CS_UNREACHABLE(...) UNREACHABLE(__VA_ARGS__)
#define CS_DEBUG_ASSERT_VAL(...) DEBUG_ASSERT_VAL(__VA_ARGS__)
#define CS_ASSUME_VAL(...) ASSUME_VAL(__VA_ARGS__)
#define CS_ASSERT_VAL(...) ASSERT_VAL(__VA_ARGS__)
