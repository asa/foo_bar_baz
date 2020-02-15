#include "net/ws.hh"

namespace net {
namespace ws {

auto decode_and_dispatch_message(const string& msg) -> effect_t {
    if (startswith(msg, "db_get")) {
        size_t id = stoi(split(msg, ' ').at(1));
        return [id](auto&& ctx) {
            ctx.dispatch(net::api::request::get_some_db_data{id});  //
        };
    }
    if (msg == "healthz") {
        return [](auto&& ctx) {
            ctx.dispatch(net::api::request::check_healthz{});  //
        };
    }
    return lager::noop;
};

auto encode_and_dispatch_message(const string& msg) -> effect_t {
    cerr << "encode message is a noop" << endl;
    return lager::noop;
};

auto dispatch_effect(action action) -> effect_t {
    return scelta::match(
        [&](send a) -> effect_t {
            cerr << "dispatching websocket: " << a.msg << endl;
            return encode_and_dispatch_message(a.msg);
        },
        [&](recv a) -> effect_t {
            cerr << "recv on websocket: " << a.msg << endl;
            return decode_and_dispatch_message(a.msg);
        })(std::move(action));
}
}  // namespace ws
}  // namespace net
