#pragma once
#include <cereal/cereal.hpp>
#include <lager/store.hpp>
#include "common/common.hh"

#include <vector>
#include "net/api/api.hh"
#include "net/api/codec.hh"
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//  handles deserialization of messages off the wire and dispatching
//  into the context
/////
namespace net {
namespace ws {

using api::codec::opcode_t;
using std::vector;
using data_t = vector<unsigned char>;

// this action is called when we want to write directly to the wire
struct send {
    opcode_t opcode;
    data_t data;
};

// read directly from the wire
struct recv {
    opcode_t opcode;
    data_t data;
};

struct encode {
    net::api::action action;
};

using action = variant<send, recv, encode>;

using effect_t = lager::effect<lager::actions<action, net::api::action>>;

auto dispatch_effect(action action) -> effect_t;

}  // namespace ws
}  // namespace net
