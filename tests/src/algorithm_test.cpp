#include <boost/ut.hpp>  // single header
// import boost.ut;        // single module (C++20)
#include <array>
#include <numeric>
#include "aal/algorithm.hpp"
/**
 * multiply all the parameter pack values together
 * @note if sizeof...(a) == 1 it returns a.
 */
static constexpr auto multiply = [](const auto &...a) { return (a * ...); };

/**
 * add all the parameter pack values together
 * @note if sizeof...(a) == 1 it returns a.
 */
static constexpr auto plus = [](const auto &...a) { return (a + ...); };

/**
 * are all the parameter pack values equal?
 * @param first value
 * @param rest of values.
 */
static constexpr auto equal_to = [](const auto &first, const auto &...rest) -> bool {
    static_assert(sizeof...(rest) != 0);
    return ((first == rest) || ...);
};

/**
 * are any of the parameter pack values not equal?
 * @param first value
 * @param rest of values.
 */
static constexpr auto not_equal_to = [](const auto &first, const auto &...rest) -> bool {
    static_assert(sizeof...(rest) != 0);
    return ((first != rest) || ...);
};

/**
 * Transform values to signed types and pass to op
 * @param op operation that is performed that returns a value.
 * @param a values
 */
static constexpr auto make_signed = [](const auto &op, const auto &...a) {
    static_assert((std::is_integral_v<std::decay_t<decltype(a)>> && ...));
    return op(static_cast<std::make_signed_t<std::decay_t<decltype(a)>>>(a)...);
};

/**
 * Transform values to unsigned types and pass to op
 * @param op operation that is performed that returns a value.
 * @param a values
 */
static constexpr auto make_unsigned = [](const auto &op, const auto &...a) {
    static_assert((std::is_integral_v<std::decay_t<decltype(a)>> && ...));
    return op((static_cast<std::make_unsigned_t<std::decay_t<decltype(a)>>>(a))...);
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

template <typename I, typename... Is, typename O, typename Op>
requires requires(O &&o) {
    std::begin(o);
}
[[nodiscard]] consteval auto
transform(const Op op, O &&o, I f, const I l, Is... fs) {
    aal::var::transform(op, std::begin(o), f, l, fs...);
    return o;
}

/**
 * Eager constant time construct a std::array of numbers using iota.
 * @tparam start number
 * @tparam count of values
 * @return std::array from start to start+count
 */
template <auto start, std::size_t count>
requires(count < std::numeric_limits<decltype(start)>::max() - start) [[nodiscard]] consteval auto iota() {
    // std::iota is not constexpr in clang.
    std::array<decltype(start), count> iota_range{};
    auto i = start;
    aal::var::transform([&i]([[maybe_unused]] const auto &input) { return i++; },
                        std::begin(iota_range),
                        std::begin(iota_range),
                        std::end(iota_range));
    return iota_range;
}

int
main() {
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;
    using namespace boost::ut;
    [[maybe_unused]] suite tests = [] {
        static constexpr auto input = iota<0, 10U>();
        static constexpr auto b     = std::begin(input);
        static constexpr auto e     = std::end(input);

        "plus"_test = [] {
            expect(1_i == plus(1));
            expect(0_i == plus(1, -1));
            expect(3_i == plus(1, 1, 1));
            expect(957_i == plus(100, 158, 699));
        };

        "multiply"_test = [] {
            expect(1_i == multiply(1));
            expect(-1_i == multiply(1, -1));
            expect(1_i == multiply(1, 1, 1));
            expect(11044200_i == multiply(100, 158, 699));
        };

        "equal_to"_test = [] {
            expect(constant<equal_to(0, 0)>);
            expect(equal_to(0.0, 0));
            expect(make_signed(equal_to, 0U, 0));
            expect(make_unsigned(equal_to, 0U, 0));
            expect(make_signed(equal_to, 3, 3U, 3U, 3));
            expect(make_signed(equal_to, 3, 3U, 3));
            expect(make_unsigned(equal_to, 3, 3U, 3, char(3)));
        };

        "not_equal_to"_test = [] {
            expect(constant<not_equal_to(3, 3, 6, 5, 3)>);
            expect(make_signed(not_equal_to, -3, 3U, 6U, '5', 3));
            expect(!make_signed(not_equal_to, 3, 3U, 3U, 3, char(3)));
            expect(!make_signed(not_equal_to, 3, 3U, 3));
            expect(!make_unsigned(not_equal_to, 3, 3U, 3));
        };

        "find"_test = [] {
            static constexpr auto result = aal::var::find([&](const auto &...a) { return equal_to(3, a...); }, b, e);
            expect(constant<e != std::get<0>(result)>);
            expect(constant<3 == *std::get<0>(result)>);
        };

        "any_of"_test = [] {
            static constexpr auto result = aal::var::any_of([&](const auto &...a) { return equal_to(3, a...); }, b, e);
            expect(constant<result>);
        };

        "transform == 3"_test = [] {
            std::array<int, sizeof(input)> output{};
            const auto result =
              aal::var::transform([&](const auto &...a) { return equal_to(3, a...); }, std::begin(output), b, e);
            expect(output[3]);
            expect(!*result);
        };

        "transform == 3 constexpr"_test = [] {
            constexpr auto result =
              transform([&](const auto &...a) { return equal_to(3, a...); }, std::array<int, sizeof(input)>(), b, e);
            expect(constant<result[3]>);
        };

        "transform plus 3"_test = [&] {
            std::array<int, sizeof(input)> result{};
            aal::var::transform([&](const auto &...a) { return plus(3, a...); }, std::begin(result), b, e);
            expect(result[3] == 6_i);
        };

        "transform == 3 constexpr"_test = [&] {
            constexpr auto result =
              transform([&](const auto &...a) { return plus(3, a...); }, std::array<int, sizeof(input)>(), b, e);
            expect(constant<result[3] == 6>);
            expect(constant<result[2] == 5>);
            expect(constant<result[1] == 4>);
        };

        "transform plus 2 times"_test = [] {
            std::array<int, sizeof(input)> result{};
            aal::var::transform(plus, std::begin(result), b, e, b);
            expect(result[3] == 6_i);
            expect(result[2] == 4_i);
            expect(result[1] == 2_i);
        };

        "transform plus 2 times constexpr"_test = [] {
            constexpr auto result = transform(plus, std::array<int, sizeof(input)>(), b, e, b);
            expect(constant<result[3] == 6>);
            expect(constant<result[2] == 4>);
            expect(constant<result[1] == 2>);
        };

        "transform plus 3 times"_test = [] {
            std::array<int, sizeof(input)> result{};
            aal::var::transform(plus, std::begin(result), b, e, b, b);
            expect(result[3] == 9_i);
            expect(result[2] == 6_i);
            expect(result[1] == 3_i);
        };

        "transform plus 3 times constexpr"_test = [] {
            constexpr auto result = transform(plus, std::array<int, sizeof(input)>(), b, e, b, b);
            expect(constant<result[3] == 9>);
            expect(constant<result[2] == 6>);
            expect(constant<result[1] == 3>);
        };

        "transform multiply 3 times"_test = [] {
            std::array<int, sizeof(input)> result{};
            aal::var::transform(multiply, std::begin(result), b, e, b, b);
            expect(result[3] == 27_i);
            expect(result[2] == 8_i);
            expect(result[1] == 1_i);
        };

        "transform multiply 3 times constexpr"_test = [] {
            constexpr auto result = transform(multiply, std::array<int, sizeof(input)>(), b, e, b, b);
            expect(constant<result[3] == 27>);
            expect(constant<result[2] == 8>);
            expect(constant<result[1] == 1>);
        };
    };
}