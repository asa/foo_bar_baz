#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <lager/debug/cereal/struct.hpp>
#include "common/common.hh"

#include "net/api/codec.hh"

using std::byte;
using std::vector;

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
using data_t = vector<unsigned char>;

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

inline auto decode(opcode_t opcode, const data_t& data) -> net::api::action {
    switch (opcode) {
        case 1:
            cerr << "decoded request::get_some_db_data{}" << endl;
            return net::api::request::get_some_db_data{};  //
        case 2:
            cerr << "decoded request::check_healthz{}" << endl;
            return net::api::request::check_healthz{};  //
        case 3:
            cerr << "decoded response::db_data{}" << endl;
            return net::api::response::db_data{};  //
        case 4:
            cerr << "decoded response::healthz{}" << endl;
            return net::api::response::healthz{};  //
    }
}

inline auto encode(net::api::action action) -> data_t {
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
        })(std::move(action));

    const auto str = ss.str();
    const data_t data(str.begin(), str.end());

    return data;
}
}  // namespace codec

}  // namespace api
}  // namespace net
