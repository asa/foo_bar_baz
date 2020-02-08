#include "app/core.hh"

namespace core {

auto update(app_model m, app_action action) -> app_result {
    auto eff = [](auto&& ctx) { cerr << "app effect happens" << endl; };

    return scelta::match(
        [&](foo::action a) -> app_result {
            auto [new_foo, eff] = foo::update(m.foo, a);
            m.foo = new_foo;
            return {m, eff};  //
        },
        [&](bar::action a) -> app_result {
            auto [new_bar, eff] = bar::update(m.bar, a);
            m.bar = new_bar;
            return {m, eff};
        },
        [&](baz::action a) -> app_result {
            auto [new_baz, eff] = baz::update(m.baz, a);
            m.baz = new_baz;
            return {m, eff};  //
        })(std::move(action));
}

}  // namespace core

/*
   [](auto&& ctx) {
                           auto& io = get<boost::asio::io_context>(ctx);
                           io.post([] {
                               cerr << "running something directly into the ios " << endl;  //
                           });
                       }
*/
