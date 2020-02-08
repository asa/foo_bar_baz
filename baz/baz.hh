#pragma once
#include <lager/store.hpp>
#include "common/common.hh"

namespace baz {

////////////////////////////    model    /////////////////////////////
struct model {
    int baz_id;
};

////////////////////////////    actions     /////////////////////////////
struct baz_a_action {};
struct baz_b_action {};

using action = std::variant<baz_a_action, baz_b_action>;

using result = std::pair<model, lager::effect<action>>;

////////////////////////////    reducers    ///////////////////////////////

auto update(model m, action a) -> result;

}  // namespace baz
