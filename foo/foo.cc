#include "foo/foo.hh"
#include "bar/bar.hh"

namespace foo {

auto update(model m, action a) -> result {
    auto eff = [](auto&& ctx) { cerr << "foo effect" << endl; };

    return scelta::match(
        [&](foo_a_action a) -> result {
            cerr << "do foo a action" << endl;
            return {m, [](auto&& ctx) {
                        ctx.dispatch(bar::bar_a_action{});  //
                    }};
        },
        [&](foo_b_action a) -> result {
            cerr << "do foo b action" << endl;
            return {m, lager::noop};
        })(std::move(a));
}

}  // namespace foo
