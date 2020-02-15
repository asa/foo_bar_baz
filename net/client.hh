#pragma once
#include "common/common.hh"

#include "foo/foo.hh"
#include "net/api.hh"

#include <lager/store.hpp>
#include <scelta.hpp>

namespace net {
namespace client {
////////////////////////////    model    /////////////////////////////

using error_t = string;

namespace state {

struct idle {};

struct connected {
    string host;
    int port;
};

struct error {
    error_t ec;
};
}  // namespace state

using status_t = variant<state::idle, state::connected, state::error>;

struct model {
    status_t status;
};

///////////////////////////    actions     /////////////////////////////

struct connect_action {
    string host;
    int port;
};

struct disconnect_action {};

struct error_action {
    error_t ec;
};

using connection_action = variant<connect_action,     //
                                  disconnect_action,  //
                                  error_action        //
                                  >;
using connection_result = pair<status_t, lager::effect<connection_action>>;

using action = variant<
    // connection_action,  //
    api::action>;

using api_result = pair<model,                                       //
                        lager::effect<                               //
                            lager::actions<api::request::requests,   //
                                           api::response::responses  //
                                                                     // ,  foo::action
                                           >>>;                      //

using result = pair<model, lager::effect<action>>;

////////////////////////////    reducers    ///////////////////////////////

// auto update(status_t m, connection_action a) -> connection_result;

auto update_api_request(model m, api::request::requests a) -> api_result;

auto update_api_response(model m, api::response::responses a) -> api_result;

auto update_client_api(model m, api::action a) -> api_result;

auto update(model m, action a) -> result;

}  // namespace client
}  // namespace net
