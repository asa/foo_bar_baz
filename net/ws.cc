#include "net/ws.hh"
#include <cereal/archives/binary.hpp>
//#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <sstream>

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

auto encode_and_dispatch_message(net::api::action a) -> effect_t {
    cerr << "encode a message" << endl;

    std::stringstream ss;

    cereal::BinaryOutputArchive archive(ss);

    archive(a);

    archive("blah");
    cerr << ss.str() << endl;

    return [](auto&& ctx) {
        // ctx.dispatch(net::ws::send{"some serialized message"});  //
    };
};

auto dispatch_effect(action action) -> effect_t {
    return scelta::match(
        [&](encode a) -> effect_t {
            cerr << "net api action to encode " << endl;
            return encode_and_dispatch_message(a.action);
        },
        [&](send a) -> effect_t {
            cerr << "dispatching websocket: " << a.msg << endl;
            return lager::noop;
        },
        [&](recv a) -> effect_t {
            cerr << "recv on websocket: " << a.msg << endl;
            return decode_and_dispatch_message(a.msg);
        })(std::move(action));
}
}  // namespace ws
}  // namespace net
