#pragma once
#include <lager/store.hpp>
#include "common/common.hh"

namespace bar {

////////////////////////////    model    /////////////////////////////
struct model {
    int bar_id;
    string data;
};

////////////////////////////    actions     /////////////////////////////
struct bar_a_action {
    string data;
};
struct bar_b_action {};

using action = std::variant<bar_a_action, bar_b_action>;

using result = std::pair<model, lager::effect<action>>;

////////////////////////////    reducers    ///////////////////////////////

auto update(model m, action a) -> result;

}  // namespace bar
