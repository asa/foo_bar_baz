#include "app/core.hh"

namespace core {

auto update(app_model m, app_action action) -> app_result {
    auto eff = [](auto&& ctx) {
        cerr << std::this_thread::get_id() << endl;
        cerr << "app effect happens" << endl;
    };

    return scelta::match(
        [&](foo::action a) -> app_result {
            auto [new_foo, eff] = foo::update(m.foo, a);
            m.foo = new_foo;
            cerr << std::this_thread::get_id() << endl;
            return {std::move(m), eff};  //
        },
        [&](bar::action a) -> app_result {
            auto [new_bar, eff] = bar::update(m.bar, a);
            m.bar = new_bar;
            cerr << std::this_thread::get_id() << endl;
            return {std::move(m), eff};
        },
        [&](baz::action a) -> app_result {
            auto [new_baz, eff] = baz::update(m.baz, a);
            m.baz = new_baz;
            cerr << std::this_thread::get_id() << endl;
            return {std::move(m), [](auto&& ctx) {
                        auto& io = get<boost::asio::io_context>(ctx);
                        cerr << std::this_thread::get_id() << endl;
                        io.post([] { cerr << "running something directly into the ios " << endl; });
                    }};
        },
        [&](net::client::action a) -> app_result {
            auto [new_net_client, eff] = net::client::update(m.net_client, a);
            m.net_client = new_net_client;
            return {std::move(m), eff};
        })(std::move(action));
}  // namespace core

}  // namespace core
