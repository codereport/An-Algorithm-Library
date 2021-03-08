#include <boost/ut.hpp> // single header
// import boost.ut;        // single module (C++20)
#include "aal/algorithm.hpp"
#include <array>
#include <ranges>
/**
 * multiply all the parameter pack values together
 * @note if sizeof...(a) == 1 it returns a.
 */
static constexpr auto multiply = [](auto const &...a) { return (a * ...); };
/**
 * add all the parameter pack values together
 * @note if sizeof...(a) == 1 it returns a.
 */
static constexpr auto plus = [](auto const &...a) { return (a + ...); };
/**
 * are all the parameter pack values equal?
 * @param first value
 * @param rest of values.
 */
static constexpr auto equal_to = [](auto const &first,
                                    auto const &...rest) -> bool {
  static_assert(sizeof...(rest) != 0);
  return ((first == rest) || ...);
};
/**
 * are any of the parameter pack values not equal?
 * @param first value
 * @param rest of values.
 */
static constexpr auto not_equal_to = [](auto const &first,
                                        auto const &...rest) -> bool {
  static_assert(sizeof...(rest) != 0);
  return ((first != rest) || ...);
};

/**
 * Transform values to signed types and pass to op
 * @param op operation that is performed that returns a value.
 * @param a values
 */
static constexpr auto make_signed = [](const auto &op, auto const &...a) {
  static_assert((std::is_integral_v<std::decay_t<decltype(a)>> && ...));
  return op(static_cast<std::make_signed_t<std::decay_t<decltype(a)>>>(a)...);
};
/**
 * Transform values to unsigned types and pass to op
 * @param op operation that is performed that returns a value.
 * @param a values
 */
static constexpr auto make_unsigned = [](const auto &op, auto const &...a) {
  static_assert((std::is_integral_v<std::decay_t<decltype(a)>> && ...));
  return op(
      (static_cast<std::make_unsigned_t<std::decay_t<decltype(a)>>>(a))...);
};
/**
 * Eager transform and output to rvalue range
 * @tparam I first range iterator type
 * @tparam Is other range iterators type
 * @tparam O output range r-value type
 * @tparam Op operation type
 * @param op operation
 * @param o output range r-value
 * @param f first range iterator begin
 * @param l first range iterator end
 * @param fs other range iterator begins
 * @return returns output range post transform.
 */
template <typename I, typename... Is, std::ranges::range O, typename Op>
[[nodiscard]] constexpr auto transform(Op const op, O &&o, I f, I const l,
                                       Is... fs) {
  aal::var::transform(op, std::ranges::begin(o), f, l, fs...);
  return o;
}
int main() {
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  [[maybe_unused]] suite tests = [] {
    static constexpr std::array input{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    };
    static constexpr auto b = std::ranges::begin(input);
    static constexpr auto e = std::ranges::end(input);
    "equal_to"_test = [&] {
      expect(constant<equal_to(0, 0)>);
      expect(equal_to(0.0, 0));
      expect(make_signed(equal_to, 0U, 0));
      expect(make_unsigned(equal_to, 0U, 0));
      expect(make_signed(equal_to, 3, 3U, 3U, 3));
      expect(make_signed(equal_to, 3, 3U, 3));
      expect(make_unsigned(equal_to, 3, 3U, 3, char(3)));
    };
    "not_equal_to"_test = [&] {
      expect(constant<not_equal_to(3, 3, 6, 5, 3)>);
      expect(make_signed(not_equal_to, -3, 3U, 6U, '5', 3));
      expect(!make_signed(not_equal_to, 3, 3U, 3U, 3, char(3)));
      expect(!make_signed(not_equal_to, 3, 3U, 3));
      expect(!make_unsigned(not_equal_to, 3, 3U, 3));
    };
    "find"_test = [&] {
      static constexpr auto result = aal::var::find(
          [&](const auto &...a) { return equal_to(3, a...); }, b, e);
      expect(constant<e != std::get<0>(result)>);
      expect(constant<3 == *std::get<0>(result)>);
    };
    "found"_test = [&] {
      static constexpr auto result = aal::var::found(
          [&](const auto &...a) { return equal_to(3, a...); }, b, e);
      expect(constant<result>);
    };

    "transform == 3"_test = [&] {
      std::array<int, sizeof(input)> output{};
      const auto result = aal::var::transform(
          [&](const auto &...a) { return equal_to(3, a...); },
          std::ranges::begin(output), b, e);
      expect(output[3]);
      expect(!*result);
    };
    "transform == 3 constexpr"_test = [&] {
      constexpr auto result =
          transform([&](const auto &...a) { return equal_to(3, a...); },
                    std::array<int, sizeof(input)>(), b, e);
      expect(constant<result[3]>);
    };
    "transform + 3"_test = [&] {
      std::array<int, sizeof(input)> result{};
      aal::var::transform([&](const auto &...a) { return plus(3, a...); },
                          std::ranges::begin(result), b, e);
      expect(result[3] == 6_i);
    };
    "transform == 3 constexpr"_test = [&] {
      constexpr auto result =
          transform([&](const auto &...a) { return plus(3, a...); },
                    std::array<int, sizeof(input)>(), b, e);
      expect(constant<result[3] == 6>);
      expect(constant<result[2] == 5>);
      expect(constant<result[1] == 4>);
    };
    "transform + 2x"_test = [&] {
      std::array<int, sizeof(input)> result{};
      aal::var::transform(plus, std::ranges::begin(result), b, e, b);
      expect(result[3] == 6_i);
      expect(result[2] == 4_i);
      expect(result[1] == 2_i);
    };
    "transform + 2x constexpr"_test = [&] {
      constexpr auto result =
          transform(plus, std::array<int, sizeof(input)>(), b, e, b);
      expect(constant<result[3] == 6>);
      expect(constant<result[2] == 4>);
      expect(constant<result[1] == 2>);
    };
    "transform + 3x"_test = [&] {
      std::array<int, sizeof(input)> result{};
      aal::var::transform(plus, std::ranges::begin(result), b, e, b,b);
      expect(result[3] == 9_i);
      expect(result[2] == 6_i);
      expect(result[1] == 3_i);
    };
    "transform + 3x constexpr"_test = [&] {
      constexpr auto result =
          transform(plus, std::array<int, sizeof(input)>(), b, e, b,b);
      expect(constant<result[3] == 9>);
      expect(constant<result[2] == 6>);
      expect(constant<result[1] == 3>);
    };
  };
}