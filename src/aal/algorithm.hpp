#pragma once
#include <tuple>

namespace aal {
namespace var {

    template <typename I, typename... Is, typename P>
    [[nodiscard]] constexpr auto 
    find(P pred, I f, I l, Is... fs) {
        while (f != l) {
            if (pred(*f, *fs...)) break;
            ++f, (++fs,...);
        }
        return std::tuple{f, fs...};
    }

    template <typename I, typename... Is, typename P>
    [[nodiscard]] constexpr auto 
    found(P pred, I f, I l, Is... fs) {
        auto const t = find(pred, f, l, fs...);
        return std::get<0>(t) != l;
    }

}
}
