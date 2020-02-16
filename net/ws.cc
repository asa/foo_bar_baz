#include "net/ws.hh"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
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
    // TODO actually  the data
    // auto action = decode(opcode, data);
    switch (opcode) {
        case 1:
            cerr << "decoded request::get_some_db_data{}" << endl;
            return dispatch(net::api::request::get_some_db_data{});  //
        case 2:
            cerr << "decoded request::check_healthz{}" << endl;
            return dispatch(net::api::request::check_healthz{});  //
        case 3:
            cerr << "decoded response::db_data{}" << endl;
            return dispatch(net::api::response::db_data{});  //
        case 4:
            cerr << "decoded response::healthz{}" << endl;
            return dispatch(net::api::response::healthz{});  //
    }
    return lager::noop;
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
    std::stringstream ss;

    // cereal::BinaryOutputArchive archive(ss);
    cereal::JSONOutputArchive archive(ss);

    // this match is necessary as a way to collapse from nested variants to calling archive on the struct / type
    // TODO find a way to call this templated on the real underlying current type of the variant
    // otherwise this is cumbersome to maintain
    scelta::match(  //
        [&](api::request::requests a) {
            scelta::match(                                                          //
                [&](api::request::get_some_db_data a) { archive(CEREAL_NVP(a)); },  //
                [&](api::request::check_healthz a) { archive(CEREAL_NVP(a)); })(std::move(a));
        },
        [&](api::response::responses a) {
            scelta::match(                                                  //
                [&](api::response::db_data a) { archive(CEREAL_NVP(a)); },  //
                [&](api::response::healthz a) { archive(CEREAL_NVP(a)); })(std::move(a));
        })(std::move(a));

    const auto str = ss.str();
    const data_t data(str.begin(), str.end());

    // TODO remove string here, its just for debug print
    return [opcode = net::api::to_opcode(a), data = data, str](auto&& ctx) {
        cerr << "encoded and dispatching opcode " << std::to_string(int(opcode)) << " " << str << endl;
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
