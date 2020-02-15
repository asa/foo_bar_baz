#include "foo/foo.hh"
#include "bar/bar.hh"

namespace foo {

auto update(model m, action a) -> result {
    auto eff = [](auto&& ctx) { cerr << "foo effect" << endl; };

    return scelta::match(
        [&](request_db_data_action a) -> result {
            cerr << "request some db data" << endl;
            return {std::move(m), [id = m.current_id](auto&& ctx) {
                        ctx.dispatch(net::api::request::get_some_db_data{id});  //
                    }};
        },
        [&](handle_db_data_action a) -> result {
            cerr << "handle db data action" << endl;
            m.data = a.data;
            return {std::move(m), [data = m.data](auto&& ctx) {
                        cerr << "dispatching a bar_a_action to use this db data" << endl;
                        ctx.dispatch(bar::bar_a_action{data});  //
                    }};
        })(std::move(a));
}

}  // namespace foo
