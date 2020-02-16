#pragma once
#include <cereal/cereal.hpp>
#include "common/common.hh"

namespace net {
namespace api {

// these actions make up the api between the client and the data svc
///////////////////////////    actions     /////////////////////////////

namespace request {

struct get_some_db_data {
    size_t id;
};

struct check_healthz {};

using requests = variant<request::get_some_db_data,  //
                         request::check_healthz      //
                         >;

}  // namespace request

namespace response {

struct db_data {
    string data;
};

struct healthz {
    bool ok;
};

using responses = variant<response::db_data,  //
                          response::healthz   //
                          >;

}  // namespace response

// action gathers up requests and responses
using action = variant<request::requests, response::responses>;

}  // namespace api
}  // namespace net
