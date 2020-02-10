#include "foo/foo.hh"
#include "bar/bar.hh"

namespace foo {

auto update(model m, action a) -> result {
    auto eff = [](auto&& ctx) { cerr << "foo effect" << endl; };

    return scelta::match(
        [&](foo_a_action a) -> result {
            cerr << "do foo a action" << endl;
            return {std::move(m), lager::noop};
        },
        [&](foo_b_action a) -> result {
            cerr << "_________do foo b action" << endl;
            return {std::move(m), [](auto&& ctx) {
                        cerr << "dispatching a bar_a_action" << endl;
                        ctx.dispatch(bar::bar_a_action{});  //
                    }};
        })(std::move(a));
}

}  // namespace foo
