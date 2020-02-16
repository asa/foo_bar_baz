#include "net/ws/ws.hh"

#include <iomanip>
#include <sstream>

namespace net {
namespace ws {

auto dispatch = [](auto&& action) {
    return [=](auto&& ctx) {
        ctx.dispatch(action);  //
    };
};

// TODO move this into the api area so we can directly keep it in sync
auto decode_and_dispatch_message(opcode_t opcode, const data_t& data) -> effect_t {
    auto action = net::api::codec::decode(opcode, data);
    return dispatch(action);
};

// TODO move this into the api area so we can directly keep it in sync
auto send_websocket_down_the_wire(net::ws::send a) -> effect_t {
    //
    return lager::noop;
};

/*
 *  handle encoding of actions -> raw websocket messages (opcode,data)
 *  which is then sent out over ws::send
 */
auto encode_and_dispatch_message(net::api::action a) -> effect_t {
    const auto opcode = net::api::codec::to_opcode(a);
    const auto data = net::api::codec::encode(a);

    // TODO remove string here, its just for debug print
    return [opcode, data](auto&& ctx) {
        cerr << "encoded and dispatching opcode " << std::to_string(int(opcode)) << endl;
        ctx.dispatch(net::ws::send{opcode, data});  //
    };
};

auto dispatch_effect(action action) -> effect_t {
    return scelta::match(
        [&](encode a) -> effect_t {
            cerr << "net api action to encode " << endl;
            return encode_and_dispatch_message(a.action);
        },
        [&](send a) -> effect_t {
            cerr << "dispatching websocket: opcode: " << int(a.opcode) << endl;
            return send_websocket_down_the_wire(a);
        },
        [&](recv a) -> effect_t {
            cerr << "recv on websocket wire : opcode: " << int(a.opcode) << endl;
            return decode_and_dispatch_message(a.opcode, a.data);
        })(std::move(action));
}
}  // namespace ws
}  // namespace net
