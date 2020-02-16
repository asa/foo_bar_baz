#pragma once
#include <lager/store.hpp>
#include "bar/bar.hh"
#include "common/common.hh"
#include "net/api/api.hh"

namespace foo {

////////////////////////////    model    /////////////////////////////
struct model {
    size_t current_id;
    string data;
};

////////////////////////////    actions     /////////////////////////////
struct request_db_data_action {};

struct handle_db_data_action {
    string data;
};

using action = std::variant<request_db_data_action, handle_db_data_action>;

using result = std::pair<model, lager::effect<lager::actions<net::api::action, bar::action, action>>>;

////////////////////////////    reducers    ///////////////////////////////

auto update(model m, action a) -> result;

}  // namespace foo
