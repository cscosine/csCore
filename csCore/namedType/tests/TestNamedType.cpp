// Note: copied fom original NamedType/test/tests.cpp
// replaced fluent:: with csCore::namedType::

// Cleanup windows include - windows.h is included by "catch.hpp" -->

#define NOGDI

// <-- Cleanup windows include

#include <catch2/catch_all.hpp>

#include <csCore/namedType/NamedType.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

// Usage examples

template <typename T>
decltype(auto) tee(T&& value) {
  std::cout << value << '\n';
  return std::forward<T>(value);
}

using Meter =
    csCore::namedType::NamedType<unsigned long long, struct MeterParameter, csCore::namedType::Addable, csCore::namedType::Comparable>;
constexpr Meter operator"" _meter(unsigned long long value) {
  return Meter(value);
}

using Width = csCore::namedType::NamedType<Meter, struct WidthParameter>;
using Height = csCore::namedType::NamedType<Meter, struct HeightParameter>;

class Rectangle {
public:
  Rectangle(Width width, Height height)
      : width_(width.get())
      , height_(height.get()) {}
  Meter getWidth() const {
    return width_;
  }
  Meter getHeight() const {
    return height_;
  }

private:
  Meter width_;
  Meter height_;
};

TEST_CASE("Basic usage") {
  Rectangle r(Width(10_meter), Height(12_meter));
  REQUIRE(r.getWidth().get() == 10);
  REQUIRE(r.getHeight().get() == 12);
}

using NameRef = csCore::namedType::NamedType<std::string&, struct NameRefParameter>;

void changeValue(NameRef name) {
  name.get() = "value2";
}

TEST_CASE("Passing a strong reference") {
  std::string value = "value1";
  changeValue(NameRef(value));
  REQUIRE(value == "value2");
}

TEST_CASE("Construction of NamedType::ref from the underlying type") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag>;
  auto addOne = [](StrongInt::ref si) {
    ++(si.get());
  };

  int i = 42;
  addOne(StrongInt::ref(i));
  REQUIRE(i == 43);
}

TEST_CASE("Implicit conversion of NamedType to NamedType::ref") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag>;
  auto addOne = [](StrongInt::ref si) {
    ++(si.get());
  };

  StrongInt i(42);
  addOne(i);
  REQUIRE(i.get() == 43);

  StrongInt j(42);
  addOne(StrongInt::ref(j));
  REQUIRE(j.get() == 43);
}

struct PotentiallyThrowing {
  PotentiallyThrowing() {}
};

struct NonDefaultConstructible {
  NonDefaultConstructible(int) {}
};

struct UserProvided {
  UserProvided();
};
UserProvided::UserProvided() = default;

TEST_CASE("Default construction") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag>;
  StrongInt strongInt;
  strongInt.get() = 42;
  REQUIRE(strongInt.get() == 42);
  static_assert(std::is_nothrow_constructible<StrongInt>::value, "StrongInt is not nothrow constructible");

  // Default constructible
  static_assert(std::is_default_constructible<StrongInt>::value, "StrongInt is not default constructible");
  using StrongNonDefaultConstructible = csCore::namedType::NamedType<NonDefaultConstructible, struct StrongNonDefaultConstructibleTag>;
  static_assert(!std::is_default_constructible<StrongNonDefaultConstructible>::value,
                "StrongNonDefaultConstructible is default constructible");

  // Trivially constructible
  static_assert(std::is_trivially_constructible<StrongInt>::value, "StrongInt is not trivially constructible");
  using StrongUserProvided = csCore::namedType::NamedType<UserProvided, struct StrongUserProvidedTag>;
  static_assert(!std::is_trivially_constructible<StrongUserProvided>::value, "StrongUserProvided is trivially constructible");

  // Nothrow constructible
  static_assert(std::is_nothrow_constructible<StrongInt>::value, "StrongInt is not nothrow constructible");
  using StrongPotentiallyThrowing = csCore::namedType::NamedType<PotentiallyThrowing, struct StrongPotentiallyThrowingTag>;
  static_assert(!std::is_nothrow_constructible<StrongPotentiallyThrowing>::value,
                "StrongPotentiallyThrowing is nothrow constructible");
}

template <typename Function>
using Comparator = csCore::namedType::NamedType<Function, struct ComparatorParameter>;

template <typename Function>
std::string performAction(Comparator<Function> comp) {
  return comp.get()();
}

TEST_CASE("Strong generic type") {
  REQUIRE(performAction(csCore::namedType::make_named<Comparator>([]() {
            return std::string("compare");
          })) == "compare");
}

TEST_CASE("Addable") {
  using AddableType = csCore::namedType::NamedType<int, struct AddableTag, csCore::namedType::Addable>;
  AddableType s1(12);
  AddableType s2(10);
  REQUIRE((s1 + s2).get() == 22);
  REQUIRE((+s1).get() == 12);
}

TEST_CASE("Addable constexpr") {
  using AddableType = csCore::namedType::NamedType<int, struct AddableTag, csCore::namedType::Addable>;
  constexpr AddableType s1(12);
  constexpr AddableType s2(10);
  static_assert((s1 + s2).get() == 22, "Addable is not constexpr");
  static_assert((+s1).get() == 12, "Addable is not constexpr");
}

TEST_CASE("BinaryAddable") {
  using BinaryAddableType = csCore::namedType::NamedType<int, struct BinaryAddableTag, csCore::namedType::BinaryAddable>;
  BinaryAddableType s1(12);
  BinaryAddableType s2(10);
  REQUIRE((s1 + s2).get() == 22);
}

TEST_CASE("BinaryAddable constexpr") {
  using BinaryAddableType = csCore::namedType::NamedType<int, struct BinaryAddableTag, csCore::namedType::BinaryAddable>;
  constexpr BinaryAddableType s1(12);
  constexpr BinaryAddableType s2(10);
  static_assert((s1 + s2).get() == 22, "BinaryAddable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BinaryAddable constexpr C++17") {
  using BinaryAddableType = csCore::namedType::NamedType<int, struct BinaryAddableTag, csCore::namedType::BinaryAddable>;
  constexpr BinaryAddableType s(10);
  static_assert(BinaryAddableType{12}.operator+=(s).get() == 22, "BinaryAddable is not constexpr");
}
#endif

TEST_CASE("UnaryAddable") {
  using UnaryAddableType = csCore::namedType::NamedType<int, struct UnaryAddableTag, csCore::namedType::UnaryAddable>;
  UnaryAddableType s1(12);
  REQUIRE((+s1).get() == 12);
}

TEST_CASE("UnaryAddable constexpr") {
  using UnaryAddableType = csCore::namedType::NamedType<int, struct UnaryAddableTag, csCore::namedType::UnaryAddable>;
  constexpr UnaryAddableType s1(12);
  static_assert((+s1).get() == 12, "UnaryAddable is not constexpr");
}

TEST_CASE("Subtractable") {
  using SubtractableType = csCore::namedType::NamedType<int, struct SubtractableTag, csCore::namedType::Subtractable>;
  SubtractableType s1(12);
  SubtractableType s2(10);
  REQUIRE((s1 - s2).get() == 2);
  REQUIRE((-s1).get() == -12);
}

TEST_CASE("Subtractable constexpr") {
  using SubtractableType = csCore::namedType::NamedType<int, struct SubtractableTag, csCore::namedType::Subtractable>;
  constexpr SubtractableType s1(12);
  constexpr SubtractableType s2(10);
  static_assert((s1 - s2).get() == 2, "Subtractable is not constexpr");
  static_assert((-s1).get() == -12, "Subtractable is not constexpr");
}

TEST_CASE("BinarySubtractable") {
  using BinarySubtractableType =
      csCore::namedType::NamedType<int, struct BinarySubtractableTag, csCore::namedType::BinarySubtractable>;
  BinarySubtractableType s1(12);
  BinarySubtractableType s2(10);
  REQUIRE((s1 - s2).get() == 2);
}

TEST_CASE("BinarySubtractable constexpr") {
  using BinarySubtractableType =
      csCore::namedType::NamedType<int, struct BinarySubtractableTag, csCore::namedType::BinarySubtractable>;
  constexpr BinarySubtractableType s1(12);
  constexpr BinarySubtractableType s2(10);
  static_assert((s1 - s2).get() == 2, "BinarySubtractable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BinarySubtractable constexpr C++17") {
  using BinarySubtractableType =
      csCore::namedType::NamedType<int, struct BinarySubtractableTag, csCore::namedType::BinarySubtractable>;
  constexpr BinarySubtractableType s(10);
  static_assert(BinarySubtractableType{12}.operator-=(s).get() == 2, "BinarySubtractable is not constexpr");
}
#endif

TEST_CASE("UnarySubtractable") {
  using UnarySubtractableType = csCore::namedType::NamedType<int, struct UnarySubtractableTag, csCore::namedType::UnarySubtractable>;
  UnarySubtractableType s(12);
  REQUIRE((-s).get() == -12);
}

TEST_CASE("UnarySubtractable constexpr") {
  using UnarySubtractableType = csCore::namedType::NamedType<int, struct UnarySubtractableTag, csCore::namedType::UnarySubtractable>;
  constexpr UnarySubtractableType s(12);
  static_assert((-s).get() == -12, "UnarySubtractable is not constexpr");
}

TEST_CASE("Multiplicable") {
  using MultiplicableType = csCore::namedType::NamedType<int, struct MultiplicableTag, csCore::namedType::Multiplicable>;
  MultiplicableType s1(12);
  MultiplicableType s2(10);
  REQUIRE((s1 * s2).get() == 120);
  s1 *= s2;
  REQUIRE(s1.get() == 120);
}

TEST_CASE("Multiplicable constexpr") {
  using MultiplicableType = csCore::namedType::NamedType<int, struct MultiplicableTag, csCore::namedType::Multiplicable>;
  constexpr MultiplicableType s1(12);
  constexpr MultiplicableType s2(10);
  static_assert((s1 * s2).get() == 120, "Multiplicable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("Multiplicable constexpr C++17") {
  using MultiplicableType = csCore::namedType::NamedType<int, struct MultiplicableTag, csCore::namedType::Multiplicable>;
  constexpr MultiplicableType s(10);
  static_assert(MultiplicableType{12}.operator*=(s).get() == 120, "Multiplicable is not constexpr");
}
#endif

TEST_CASE("Divisible") {
  using DivisibleType = csCore::namedType::NamedType<int, struct DivisibleTag, csCore::namedType::Divisible>;
  DivisibleType s1(120);
  DivisibleType s2(10);
  REQUIRE((s1 / s2).get() == 12);
  s1 /= s2;
  REQUIRE(s1.get() == 12);
}

TEST_CASE("Divisible constexpr") {
  using DivisibleType = csCore::namedType::NamedType<int, struct DivisibleTag, csCore::namedType::Divisible>;
  constexpr DivisibleType s1(120);
  constexpr DivisibleType s2(10);
  static_assert((s1 / s2).get() == 12, "Divisible is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("Divisible constexpr C++17") {
  using DivisibleType = csCore::namedType::NamedType<int, struct DivisibleTag, csCore::namedType::Divisible>;
  constexpr DivisibleType s(10);
  static_assert(DivisibleType{120}.operator/=(s).get() == 12, "Divisible is not constexpr");
}
#endif

TEST_CASE("Modulable") {
  using ModulableType = csCore::namedType::NamedType<int, struct ModulableTag, csCore::namedType::Modulable>;
  ModulableType s1(5);
  ModulableType s2(2);
  CHECK((s1 % s2).get() == 1);
  s1 %= s2;
  CHECK(s1.get() == 1);
}

TEST_CASE("Modulable constexpr") {
  using ModulableType = csCore::namedType::NamedType<int, struct ModulableTag, csCore::namedType::Modulable>;
  constexpr ModulableType s1(5);
  constexpr ModulableType s2(2);
  static_assert((s1 % s2).get() == 1, "Modulable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("Modulable constexpr C++17") {
  using ModulableType = csCore::namedType::NamedType<int, struct ModulableTag, csCore::namedType::Modulable>;
  constexpr ModulableType s(2);
  static_assert(ModulableType{5}.operator%=(s).get() == 1, "Modulable is not constexpr");
}
#endif

TEST_CASE("BitWiseInvertable") {
  using BitWiseInvertableType = csCore::namedType::NamedType<int, struct BitWiseInvertableTag, csCore::namedType::BitWiseInvertable>;
  BitWiseInvertableType s1(13);
  CHECK((~s1).get() == (~13));
}

TEST_CASE("BitWiseInvertable constexpr") {
  using BitWiseInvertableType = csCore::namedType::NamedType<int, struct BitWiseInvertableTag, csCore::namedType::BitWiseInvertable>;
  constexpr BitWiseInvertableType s1(13);
  static_assert((~s1).get() == (~13), "BitWiseInvertable is not constexpr");
}

TEST_CASE("BitWiseAndable") {
  using BitWiseAndableType = csCore::namedType::NamedType<int, struct BitWiseAndableTag, csCore::namedType::BitWiseAndable>;
  BitWiseAndableType s1(2);
  BitWiseAndableType s2(64);
  CHECK((s1 & s2).get() == (2 & 64));
  s1 &= s2;
  CHECK(s1.get() == (2 & 64));
}

TEST_CASE("BitWiseAndable constexpr") {
  using BitWiseAndableType = csCore::namedType::NamedType<int, struct BitWiseAndableTag, csCore::namedType::BitWiseAndable>;
  constexpr BitWiseAndableType s1(2);
  constexpr BitWiseAndableType s2(64);
  static_assert((s1 & s2).get() == (2 & 64), "BitWiseAndable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BitWiseAndable constexpr C++17") {
  using BitWiseAndableType = csCore::namedType::NamedType<int, struct BitWiseAndableTag, csCore::namedType::BitWiseAndable>;
  constexpr BitWiseAndableType s(64);
  static_assert(BitWiseAndableType{2}.operator&=(s).get() == (2 & 64), "BitWiseAndable is not constexpr");
}
#endif

TEST_CASE("BitWiseOrable") {
  using BitWiseOrableType = csCore::namedType::NamedType<int, struct BitWiseOrableTag, csCore::namedType::BitWiseOrable>;
  BitWiseOrableType s1(2);
  BitWiseOrableType s2(64);
  CHECK((s1 | s2).get() == (2 | 64));
  s1 |= s2;
  CHECK(s1.get() == (2 | 64));
}

TEST_CASE("BitWiseOrable constexpr") {
  using BitWiseOrableType = csCore::namedType::NamedType<int, struct BitWiseOrableTag, csCore::namedType::BitWiseOrable>;
  constexpr BitWiseOrableType s1(2);
  constexpr BitWiseOrableType s2(64);
  static_assert((s1 | s2).get() == (2 | 64), "BitWiseOrable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BitWiseOrable constexpr C++17") {
  using BitWiseOrableType = csCore::namedType::NamedType<int, struct BitWiseOrableTag, csCore::namedType::BitWiseOrable>;
  constexpr BitWiseOrableType s(64);
  static_assert(BitWiseOrableType{2}.operator|=(s).get() == (2 | 64), "BitWiseOrable is not constexpr");
}
#endif

TEST_CASE("BitWiseXorable") {
  using BitWiseXorableType = csCore::namedType::NamedType<int, struct BitWiseXorableTag, csCore::namedType::BitWiseXorable>;
  BitWiseXorableType s1(2);
  BitWiseXorableType s2(64);
  CHECK((s1 ^ s2).get() == (2 ^ 64));
  s1 ^= s2;
  CHECK(s1.get() == (2 ^ 64));
}

TEST_CASE("BitWiseXorable constexpr") {
  using BitWiseXorableType = csCore::namedType::NamedType<int, struct BitWiseXorableTag, csCore::namedType::BitWiseXorable>;
  constexpr BitWiseXorableType s1(2);
  constexpr BitWiseXorableType s2(64);
  static_assert((s1 ^ s2).get() == 66, "BitWiseXorable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BitWiseXorable constexpr C++17") {
  using BitWiseXorableType = csCore::namedType::NamedType<int, struct BitWiseXorableTag, csCore::namedType::BitWiseXorable>;
  constexpr BitWiseXorableType s(64);
  static_assert(BitWiseXorableType{2}.operator^=(s).get() == 66, "BitWiseXorable is not constexpr");
}
#endif

TEST_CASE("BitWiseLeftShiftable") {
  using BitWiseLeftShiftableType =
      csCore::namedType::NamedType<int, struct BitWiseLeftShiftableTag, csCore::namedType::BitWiseLeftShiftable>;
  BitWiseLeftShiftableType s1(2);
  BitWiseLeftShiftableType s2(3);
  CHECK((s1 << s2).get() == (2 << 3));
  s1 <<= s2;
  CHECK(s1.get() == (2 << 3));
}

TEST_CASE("BitWiseLeftShiftable constexpr") {
  using BitWiseLeftShiftableType =
      csCore::namedType::NamedType<int, struct BitWiseLeftShiftableTag, csCore::namedType::BitWiseLeftShiftable>;
  constexpr BitWiseLeftShiftableType s1(2);
  constexpr BitWiseLeftShiftableType s2(3);
  static_assert((s1 << s2).get() == (2 << 3), "BitWiseLeftShiftable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BitWiseLeftShiftable constexpr C++17") {
  using BitWiseLeftShiftableType =
      csCore::namedType::NamedType<int, struct BitWiseLeftShiftableTag, csCore::namedType::BitWiseLeftShiftable>;
  constexpr BitWiseLeftShiftableType s(3);
  static_assert(BitWiseLeftShiftableType{2}.operator<<=(s).get() == (2 << 3), "BitWiseLeftShiftable is not constexpr");
}
#endif

TEST_CASE("BitWiseRightShiftable") {
  using BitWiseRightShiftableType =
      csCore::namedType::NamedType<int, struct BitWiseRightShiftableTag, csCore::namedType::BitWiseRightShiftable>;
  BitWiseRightShiftableType s1(2);
  BitWiseRightShiftableType s2(3);
  CHECK((s1 >> s2).get() == (2 >> 3));
  s1 >>= s2;
  CHECK(s1.get() == (2 >> 3));
}

TEST_CASE("BitWiseRightShiftable constexpr") {
  using BitWiseRightShiftableType =
      csCore::namedType::NamedType<int, struct BitWiseRightShiftableTag, csCore::namedType::BitWiseRightShiftable>;
  constexpr BitWiseRightShiftableType s1(2);
  constexpr BitWiseRightShiftableType s2(3);
  static_assert((s1 >> s2).get() == (2 >> 3), "BitWiseRightShiftable is not constexpr");
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("BitWiseRightShiftable constexpr C++17") {
  using BitWiseRightShiftableType =
      csCore::namedType::NamedType<int, struct BitWiseRightShiftableTag, csCore::namedType::BitWiseRightShiftable>;
  constexpr BitWiseRightShiftableType s(3);
  static_assert(BitWiseRightShiftableType{2}.operator>>=(s).get() == (2 >> 3), "BitWiseRightShiftable is not constexpr");
}
#endif

TEST_CASE("Comparable") {
  REQUIRE((10_meter == 10_meter));
  REQUIRE(!(10_meter == 11_meter));
  REQUIRE((10_meter != 11_meter));
  REQUIRE(!(10_meter != 10_meter));
  REQUIRE((10_meter < 11_meter));
  REQUIRE(!(10_meter < 10_meter));
  REQUIRE((10_meter <= 10_meter));
  REQUIRE((10_meter <= 11_meter));
  REQUIRE(!(10_meter <= 9_meter));
  REQUIRE((11_meter > 10_meter));
  REQUIRE(!(10_meter > 11_meter));
  REQUIRE((11_meter >= 10_meter));
  REQUIRE((10_meter >= 10_meter));
  REQUIRE(!(9_meter >= 10_meter));
}

TEST_CASE("Comparable constexpr") {
  static_assert((10_meter == 10_meter), "Comparable is not constexpr");
  static_assert(!(10_meter == 11_meter), "Comparable is not constexpr");
  static_assert((10_meter != 11_meter), "Comparable is not constexpr");
  static_assert(!(10_meter != 10_meter), "Comparable is not constexpr");
  static_assert((10_meter < 11_meter), "Comparable is not constexpr");
  static_assert(!(10_meter < 10_meter), "Comparable is not constexpr");
  static_assert((10_meter <= 10_meter), "Comparable is not constexpr");
  static_assert((10_meter <= 11_meter), "Comparable is not constexpr");
  static_assert(!(10_meter <= 9_meter), "Comparable is not constexpr");
  static_assert((11_meter > 10_meter), "Comparable is not constexpr");
  static_assert(!(10_meter > 11_meter), "Comparable is not constexpr");
  static_assert((11_meter >= 10_meter), "Comparable is not constexpr");
  static_assert((10_meter >= 10_meter), "Comparable is not constexpr");
  static_assert(!(9_meter >= 10_meter), "Comparable is not constexpr");
}

TEST_CASE("ConvertibleWithOperator") {
  struct B {
    B(int x_)
        : x(x_) {}
    int x;
  };

  struct A {
    A(int x_)
        : x(x_) {}
    operator B() const {
      return B(x);
    }
    int x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::ImplicitlyConvertibleTo<B>::templ>;
  StrongA strongA(A(42));
  B b = strongA;
  REQUIRE(b.x == 42);
}

TEST_CASE("ConvertibleWithOperator constexpr") {
  struct B {
    constexpr B(int x_)
        : x(x_) {}
    int x;
  };

  struct A {
    constexpr A(int x_)
        : x(x_) {}
    constexpr operator B() const {
      return B(x);
    }
    int x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::ImplicitlyConvertibleTo<B>::templ>;
  constexpr StrongA strongA(A(42));
  constexpr B b = strongA;
  static_assert(b.x == 42, "ImplicitlyConvertibleTo is not constexpr");
}

TEST_CASE("ConvertibleWithConstructor") {
  struct A {
    A(int x_)
        : x(x_) {}
    int x;
  };

  struct B {
    B(A a)
        : x(a.x) {}
    int x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::ImplicitlyConvertibleTo<B>::templ>;
  StrongA strongA(A(42));
  B b = strongA;
  REQUIRE(b.x == 42);
}

TEST_CASE("ConvertibleWithConstructor constexpr") {
  struct A {
    constexpr A(int x_)
        : x(x_) {}
    int x;
  };

  struct B {
    constexpr B(A a)
        : x(a.x) {}
    int x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::ImplicitlyConvertibleTo<B>::templ>;
  constexpr StrongA strongA(A(42));
  constexpr B b = strongA;
  static_assert(b.x == 42, "ImplicitlyConvertibleTo is not constexpr");
}

TEST_CASE("ConvertibleToItself") {
  using MyInt = csCore::namedType::NamedType<int, struct MyIntTag, csCore::namedType::ImplicitlyConvertibleTo<int>::templ>;
  MyInt myInt(42);
  int i = myInt;
  REQUIRE(i == 42);
}

TEST_CASE("ConvertibleToItself constexpr") {
  using MyInt = csCore::namedType::NamedType<int, struct MyIntTag, csCore::namedType::ImplicitlyConvertibleTo<int>::templ>;
  constexpr MyInt myInt(42);
  constexpr int i = myInt;
  static_assert(i == 42, "ImplicitlyConvertibleTo is not constexpr");
}

TEST_CASE("Hash") {
  using SerialNumber =
      csCore::namedType::NamedType<std::string, struct SerialNumberTag, csCore::namedType::Comparable, csCore::namedType::Hashable>;

  std::unordered_map<SerialNumber, int> hashMap = {{SerialNumber{"AA11"}, 10}, {SerialNumber{"BB22"}, 20}};
  SerialNumber cc33{"CC33"};
  hashMap[cc33] = 30;
  REQUIRE(hashMap[SerialNumber{"AA11"}] == 10);
  REQUIRE(hashMap[SerialNumber{"BB22"}] == 20);
  REQUIRE(hashMap[cc33] == 30);
}

struct testFunctionCallable_A {
  testFunctionCallable_A(int x_)
      : x(x_) {}
  // ensures that passing the argument to a function doesn't make a copy
  testFunctionCallable_A(testFunctionCallable_A const&) = delete;
  testFunctionCallable_A(testFunctionCallable_A&&) = default;
  testFunctionCallable_A& operator+=(testFunctionCallable_A const& other) {
    x += other.x;
    return *this;
  }
  int x;
};

testFunctionCallable_A operator+(testFunctionCallable_A const& a1, testFunctionCallable_A const& a2) {
  return testFunctionCallable_A(a1.x + a2.x);
}

bool operator==(testFunctionCallable_A const& a1, testFunctionCallable_A const& a2) {
  return a1.x == a2.x;
}

TEST_CASE("Function callable") {
  using A = testFunctionCallable_A;
  auto functionTakingA = [](A const& a) {
    return a.x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::FunctionCallable>;
  StrongA strongA(A(42));
  const StrongA constStrongA(A(42));
  REQUIRE(functionTakingA(strongA) == 42);
  REQUIRE(functionTakingA(constStrongA) == 42);
  REQUIRE(strongA + strongA == 84);
}

struct testFunctionCallable_B {
  constexpr testFunctionCallable_B(int x_)
      : x(x_) {}
  // ensures that passing the argument to a function doesn't make a copy
  testFunctionCallable_B(testFunctionCallable_B const&) = delete;
  testFunctionCallable_B(testFunctionCallable_B&&) = default;
  constexpr testFunctionCallable_B& operator+=(testFunctionCallable_B const& other) {
    x += other.x;
    return *this;
  }
  int x;
};

constexpr testFunctionCallable_B operator+(testFunctionCallable_B const& a1, testFunctionCallable_B const& a2) {
  return testFunctionCallable_B(a1.x + a2.x);
}

constexpr bool operator==(testFunctionCallable_B const& a1, testFunctionCallable_B const& a2) {
  return a1.x == a2.x;
}

constexpr int functionTakingB(testFunctionCallable_B const& b) {
  return b.x;
}

TEST_CASE("Function callable constexpr") {
  using B = testFunctionCallable_B;

  using StrongB = csCore::namedType::NamedType<B, struct StrongATag, csCore::namedType::FunctionCallable>;
  constexpr StrongB constStrongB(B(42));
  static_assert(functionTakingB(StrongB(B(42))) == 42, "FunctionCallable is not constexpr");
  static_assert(functionTakingB(constStrongB) == 42, "FunctionCallable is not constexpr");
  static_assert(StrongB(B(42)) + StrongB(B(42)) == 84, "FunctionCallable is not constexpr");
  static_assert(constStrongB + constStrongB == 84, "FunctionCallable is not constexpr");
}

TEST_CASE("Method callable") {
  class A {
  public:
    A(int x_)
        : x(x_) {}
    A(A const&) = delete; // ensures that invoking a method doesn't make a copy
    A(A&&) = default;

    int method() {
      return x;
    }
    int constMethod() const {
      return x;
    }

  private:
    int x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::MethodCallable>;
  StrongA strongA(A(42));
  const StrongA constStrongA(A((42)));
  REQUIRE(strongA->method() == 42);
  REQUIRE(constStrongA->constMethod() == 42);
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("Method callable constexpr C++17") {
  class A {
  public:
    constexpr A(int x_)
        : x(x_) {}
    A(A const&) = delete; // ensures that invoking a method doesn't make a copy
    A(A&&) = default;

    constexpr int method() {
      return x;
    }
    constexpr int constMethod() const {
      return x;
    }

  private:
    int x;
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::MethodCallable>;
  constexpr const StrongA constStrongA(A((42)));
  static_assert(StrongA(A(42))->method() == 42, "MethodCallable is not constexpr");
  static_assert(constStrongA->constMethod() == 42, "MethodCallable is not constexpr");
}
#endif

TEST_CASE("Callable") {
  class A {
  public:
    A(int x_)
        : x(x_) {}
    A(A const&) = delete; // ensures that invoking a method or function doesn't make a copy
    A(A&&) = default;

    int method() {
      return x;
    }
    int constMethod() const {
      return x;
    }

  private:
    int x;
  };

  auto functionTakingA = [](A const& a) {
    return a.constMethod();
  };

  using StrongA = csCore::namedType::NamedType<A, struct StrongATag, csCore::namedType::Callable>;
  StrongA strongA(A(42));
  const StrongA constStrongA(A(42));
  REQUIRE(functionTakingA(strongA) == 42);
  REQUIRE(strongA->method() == 42);
  REQUIRE(constStrongA->constMethod() == 42);
}

TEST_CASE("Named arguments") {
  using FirstName = csCore::namedType::NamedType<std::string, struct FirstNameTag>;
  using LastName = csCore::namedType::NamedType<std::string, struct LastNameTag>;
  static const FirstName::argument firstName;
  static const LastName::argument lastName;
  auto getFullName = [](FirstName const& firstName_, LastName const& lastName_) //
  {
    return firstName_.get() + lastName_.get(); //
  };

  auto fullName = getFullName(firstName = "James", lastName = "Bond");
  REQUIRE(fullName == "JamesBond");
}

TEST_CASE("Named arguments in any order") {
  using FirstName = csCore::namedType::NamedType<std::string, struct FirstNameTag>;
  using LastName = csCore::namedType::NamedType<std::string, struct LastNameTag>;
  static const FirstName::argument firstName;
  static const LastName::argument lastName;

  auto getFullName =
      csCore::namedType::make_named_arg_function<FirstName, LastName>([](FirstName const& firstName_, LastName const& lastName_) {
        return firstName_.get() + lastName_.get();
      });

  auto fullName = getFullName(lastName = "Bond", firstName = "James");
  REQUIRE(fullName == "JamesBond");

  auto otherFullName = getFullName(firstName = "James", lastName = "Bond");
  REQUIRE(otherFullName == "JamesBond");
}

TEST_CASE("Named arguments with bracket constructor") {
  using Numbers = csCore::namedType::NamedType<std::vector<int>, struct NumbersTag>;
  static const Numbers::argument numbers;
  auto getNumbers = [](Numbers const& numbers_) {
    return numbers_.get();
  };

  auto vec = getNumbers(numbers = {1, 2, 3});
  REQUIRE(vec == std::vector<int>{1, 2, 3});
}

TEST_CASE("Empty base class optimization") {
  REQUIRE(sizeof(Meter) == sizeof(double));
}

using strong_int = csCore::namedType::NamedType<int, struct IntTag>;

TEST_CASE("constexpr") {
  using strong_bool = csCore::namedType::NamedType<bool, struct BoolTag>;

  static_assert(strong_bool{true}.get(), "NamedType is not constexpr");
}

struct throw_on_construction {
  throw_on_construction() {
    throw 42;
  }

  throw_on_construction(int) {
    throw "exception";
  }
};

using C = csCore::namedType::NamedType<throw_on_construction, struct throwTag>;

TEST_CASE("noexcept") {
  CHECK(noexcept(strong_int{}));
  CHECK(!noexcept(C{}));

  CHECK(noexcept(strong_int(3)));
  CHECK(!noexcept(C{5}));
}

TEST_CASE("Arithmetic") {
  using strong_arithmetic = csCore::namedType::NamedType<int, struct ArithmeticTag, csCore::namedType::Arithmetic>;
  strong_arithmetic a{1};
  strong_arithmetic b{2};

  CHECK((a + b).get() == 3);

  a += b;
  CHECK(a.get() == 3);

  CHECK((a - b).get() == 1);

  a -= b;
  CHECK(a.get() == 1);

  a.get() = 5;
  CHECK((a * b).get() == 10);

  a *= b;
  CHECK(a.get() == 10);

  CHECK((a / b).get() == 5);

  a /= b;
  CHECK(a.get() == 5);

  b = ++a;
  CHECK(a.get() == 6);
  CHECK(b.get() == 6);

  b = a++;
  CHECK(a.get() == 7);
  CHECK(b.get() == 6);
}

TEST_CASE("Version macros are defined") {
  static_assert(NAMED_TYPE_VERSION_MAJOR >= 1, "");
  static_assert(NAMED_TYPE_VERSION_MINOR >= 0, "");
  static_assert(NAMED_TYPE_VERSION_PATCH >= 0, "");

  std::stringstream ss;
  ss << NAMED_TYPE_VERSION_MAJOR << '.' << NAMED_TYPE_VERSION_MINOR << '.' << NAMED_TYPE_VERSION_PATCH;
  CHECK(ss.str() == NAMED_TYPE_VERSION);
}

TEST_CASE("Printable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::Printable>;

  std::ostringstream oss;
  oss << StrongInt(42);
  CHECK(oss.str() == "42");
}

TEST_CASE("Dereferencable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::Dereferencable>;

  {
    StrongInt a{1};
    int& value = *a;
    CHECK(value == 1);
  }

  {
    const StrongInt a{1};
    const int& value = *a;
    CHECK(value == 1);
  }

  {
    StrongInt a{1};
    int& value = *a;
    value = 2;
    CHECK(a.get() == 2);
  }

  {
    auto functionReturningStrongInt = []() {
      return StrongInt{28};
    };
    auto functionTakingInt = [](int value) {
      return value;
    };

    int value = functionTakingInt(*functionReturningStrongInt());
    CHECK(value == 28);
  }
}

TEST_CASE("Dereferencable constexpr") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::Dereferencable>;

  constexpr StrongInt a{28};
  static_assert(*a == 28, "Dereferencable is not constexpr");
  static_assert(*StrongInt{28} == 28, "Dereferencable is not constexpr");
}

TEST_CASE("PreIncrementable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PreIncrementable>;
  StrongInt a{1};
  StrongInt b = ++a;
  CHECK(a.get() == 2);
  CHECK(b.get() == 2);
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("PreIncrementable constexpr C++17") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PreIncrementable>;
  static_assert((++StrongInt{1}).get() == 2, "PreIncrementable is not constexpr");
}
#endif

TEST_CASE("PostIncrementable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PostIncrementable>;
  StrongInt a{1};
  StrongInt b = a++;
  CHECK(a.get() == 2);
  CHECK(b.get() == 1);
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("PostIncrementable constexpr C++17") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PostIncrementable>;
  static_assert((StrongInt{1} ++).get() == 1, "PostIncrementable is not constexpr");
}
#endif

TEST_CASE("PreDecrementable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PreDecrementable>;
  StrongInt a{1};
  StrongInt b = --a;
  CHECK(a.get() == 0);
  CHECK(b.get() == 0);
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("PreDecrementable constexpr C++17") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PreDecrementable>;
  static_assert((--StrongInt{1}).get() == 0, "PreDecrementable is not constexpr");
}
#endif

TEST_CASE("PostDecrementable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PostDecrementable>;
  StrongInt a{1};
  StrongInt b = a--;
  CHECK(a.get() == 0);
  CHECK(b.get() == 1);
}

#if FLUENT_CPP17_PRESENT
TEST_CASE("PostDecrementable constexpr C++17") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::PostDecrementable>;
  static_assert((StrongInt{1} --).get() == 1, "PostDecrementable is not constexpr");
}
#endif

TEST_CASE("Incrementable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::Incrementable>;
  {
    StrongInt a{1};
    StrongInt b = ++a;
    CHECK(a.get() == 2);
    CHECK(b.get() == 2);
  }
  {
    StrongInt a{1};
    StrongInt b = a++;
    CHECK(a.get() == 2);
    CHECK(b.get() == 1);
  }
}

TEST_CASE("Decrementable") {
  using StrongInt = csCore::namedType::NamedType<int, struct StrongIntTag, csCore::namedType::Decrementable>;
  {
    StrongInt a{1};
    StrongInt b = --a;
    CHECK(a.get() == 0);
    CHECK(b.get() == 0);
  }
  {
    StrongInt a{1};
    StrongInt b = a--;
    CHECK(a.get() == 0);
    CHECK(b.get() == 1);
  }
}

template <template <typename> class... Skills>
using SkilledType = csCore::namedType::NamedType<int, struct SkilledTypeTag, Skills...>;

TEST_CASE("Empty base class optimisation of skills") {
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::PreIncrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::PostIncrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::PreDecrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::PostDecrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BinaryAddable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::UnaryAddable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Addable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BinarySubtractable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::UnarySubtractable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Subtractable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Multiplicable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Divisible>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Modulable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BitWiseInvertable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BitWiseAndable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BitWiseOrable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BitWiseXorable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BitWiseLeftShiftable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::BitWiseRightShiftable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Comparable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Dereferencable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::ImplicitlyConvertibleTo>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Printable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Hashable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::FunctionCallable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::MethodCallable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Callable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Incrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Decrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::Arithmetic>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::PreIncrementable>));
  CHECK(sizeof(int) == sizeof(SkilledType<csCore::namedType::PreIncrementable>));
}
