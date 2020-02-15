#include "app/core.hh"

namespace mock_data_svc {

auto decode_message(model m, const string& msg) -> result {
    if (startswith(msg, "db_get")) {
        size_t id = stoi(split(msg, ' ').at(1));
        return {std::move(m), [id](auto&& ctx) {
                    ctx.dispatch(net::api::request::get_some_db_data{id});  //
                }};
    }
    if (msg == "healthz") {
        return {std::move(m), [](auto&& ctx) {
                    ctx.dispatch(net::api::request::check_healthz{});  //
                }};
    }
    return {std::move(m), lager::noop};
};

auto encode_message(model m, const string& msg) -> result {
    cerr << "encode message is a noop" << endl;
    return {std::move(m), lager::noop};
};

auto update(model m, action action) -> result {
    return scelta::match(
        [&](net::svc::action a) -> result {
            auto [new_svc, eff] = net::svc::update(m.svc, a);
            m.svc = new_svc;
            return {std::move(m), eff};
        },
        [&](websocket_send a) -> result {
            // auto [new_svc, eff] = net::svc::update(m.svc, a);
            cerr << "send over a websocket: " << a.msg << endl;
            return encode_message(std::move(m), a.msg);
        },
        [&](websocket_recv a) -> result {
            // auto [new_svc, eff] = net::svc::update(m.svc, a);
            // m.svc = new_svc;
            cerr << "recv on websocket: " << a.msg << endl;
            return decode_message(m, a.msg);
        })(std::move(action));
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
