#include "app/core.hh"

namespace mock_data_svc {

auto update(model m, action action) -> result {
    return scelta::match(
        [&](net::svc::action a) -> result {
            auto [new_svc, eff] = net::svc::update(m.svc, a);
            m.svc = new_svc;
            return {std::move(m), eff};
        },
        [&](net::ws::action a) -> result {
            // websocket is stateless, so we just use its effects???
            return {std::move(m), net::ws::dispatch_effect(a)};
        });
}

}  // namespace mock_data_svc

namespace core {

auto update(app_model m, app_action action) -> app_result {
    return scelta::match(
        [&](foo::action a) -> app_result {
            auto [new_foo, eff] = foo::update(m.foo, a);
            m.foo = new_foo;
            return {std::move(m), eff};  //
        },
        [&](bar::action a) -> app_result {
            auto [new_bar, eff] = bar::update(m.bar, a);
            m.bar = new_bar;
            return {std::move(m), eff};
        },
        [&](baz::action a) -> app_result {
            auto [new_baz, eff] = baz::update(m.baz, a);
            m.baz = new_baz;
            return {std::move(m), [](auto&& ctx) {
                        auto& io = get<boost::asio::io_context>(ctx);
                        // cerr << std::this_thread::get_id() << endl;
                        io.post([] { cerr << "running something directly into the ios " << endl; });
                    }};
        },
        [&](net::client::action a) -> app_result {
            auto [new_net_client, eff] = net::client::update(m.net_client, a);
            m.net_client = new_net_client;
            return {std::move(m), eff};
        })(std::move(action));
}

}  // namespace core
