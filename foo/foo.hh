#pragma once
#include <lager/store.hpp>
#include "bar/bar.hh"
#include "common/common.hh"

namespace foo {

////////////////////////////    model    /////////////////////////////
struct model {
    int foo_id;
};

////////////////////////////    actions     /////////////////////////////
struct foo_a_action {};
struct foo_b_action {};

using action = std::variant<foo_a_action, foo_b_action>;

using result = std::pair<model, lager::effect<lager::actions<bar::action, action>>>;
// using result = std::pair<model, lager::effect<action>>;

////////////////////////////    reducers    ///////////////////////////////

auto update(model m, action a) -> result;

}  // namespace foo
