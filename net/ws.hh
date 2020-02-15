#pragma once
#include <lager/store.hpp>
#include "common/common.hh"

#include "net/api.hh"

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//  handles deserialization of messages off the wire and dispatching
//  into the context
/////
namespace net {
namespace ws {

struct send {
    string msg;
};

struct recv {
    string msg;
};

using action = variant<send, recv>;

using effect_t = lager::effect<lager::actions<action, net::api::action>>;

auto dispatch_effect(action action) -> effect_t;

}  // namespace ws
}  // namespace net
