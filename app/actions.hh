#pragma once
#include <lager/store.hpp>
#include <lager/util.hpp>
#include "common/common.hh"

#include "bar/bar.hh"
#include "baz/baz.hh"
#include "foo/foo.hh"

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

namespace core {

struct app_model {
    foo::model foo;
    bar::model bar;
    baz::model baz;
};

using app_action = std::variant<foo::action,  //
                                bar::action,  //
                                baz::action>;

using app_result = std::pair<app_model, lager::effect<app_action>>;

}  // namespace core
