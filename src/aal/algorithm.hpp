#pragma once
#include <tuple>
#include <utility>

namespace aal {
namespace var {

template <typename I, typename... Is, typename P>
[[nodiscard]] constexpr auto
find(P pred, I f, I const l, Is... fs)
{
    for (;f != l;++f, ((void)++fs, ...)) {
        if (pred(*f, *fs...)) break;
    }
    return std::tuple{f, fs...};
}

template <typename I, typename... Is, typename P>
[[nodiscard]] constexpr auto
any_of(P&& pred, I&& f, I const l, Is&&... fs) {
    auto const t = find(std::forward<P>(pred), std::forward<I>(f), l,
                      std::forward<Is>(fs)...);
    return std::get<0>(t) != l;
}

template <typename I, typename... Is, typename O, typename Op>
constexpr auto
transform(Op op, O o, I f, I const l, Is... fs) {
    for (;f != l;++o, (void)++f, ((void)++fs, ...)) {
        *o = op(*f, *fs...);
    }
    return o;
}

}  // namespace var
}  // namespace aal
