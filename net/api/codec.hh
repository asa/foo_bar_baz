#pragma once
#include <cereal/cereal.hpp>
#include <lager/debug/cereal/struct.hpp>
#include "common/common.hh"

#include "net/api/codec.hh"

using std::byte;

namespace net {
namespace api {

namespace request {

// for each struct in the net api, setup its serialization
LAGER_CEREAL_STRUCT(get_some_db_data, (id));
LAGER_CEREAL_STRUCT(check_healthz);

}  // namespace request

namespace response {

LAGER_CEREAL_STRUCT(db_data, (data));
LAGER_CEREAL_STRUCT(healthz, (ok));

}  // namespace response

namespace codec {
using opcode_t = int;
// using opcode_t = uint8_t;

inline auto to_opcode(net::api::action action) -> opcode_t {
    return scelta::match(  //
        [&](api::request::requests a) -> opcode_t {
            scelta::match(                                                      //
                [&](api::request::get_some_db_data) -> opcode_t { return 1; },  //
                [&](api::request::check_healthz) -> opcode_t { return 2; })(std::move(a));
        },
        [&](api::response::responses a) -> opcode_t {
            scelta::match(                                              //
                [&](api::response::db_data) -> opcode_t { return 3; },  //
                [&](api::response::healthz) -> opcode_t { return 4; })(std::move(a));
        })(std::move(action));
}

}  // namespace codec

}  // namespace api
}  // namespace net
