#include "baz/baz.hh"

namespace baz {

auto update(model m, action a) -> result {
    auto eff = [](auto&& ctx) { cerr << "baz effect" << endl; };

    return scelta::match(
        [&](baz_a_action a) -> result {
            cerr << "do baz a action" << endl;
            return {m, lager::noop};
        },
        [&](baz_b_action a) -> result {
            cerr << "do baz b action" << endl;
            return {m, lager::noop};
        })(std::move(a));
}

}  // namespace baz
