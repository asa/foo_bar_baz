#include "bar/bar.hh"

namespace bar {

auto update(model m, action a) -> result {
    auto eff = [](auto&& ctx) { cerr << "bar effect" << endl; };

    return scelta::match(
        [&](bar_a_action a) -> result {
            cerr << "do bar a action_______________________________" << endl;
            m.data = a.data;
            return {std::move(m), lager::noop};
        },
        [&](bar_b_action a) -> result {
            cerr << "do bar b action" << endl;
            return {std::move(m), lager::noop};
        })(std::move(a));
}

}  // namespace bar
