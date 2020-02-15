#include "net/client.hh"

namespace net {
namespace client {

/*
auto update(status_t m, connection_action a) -> connection_result {
return scelta::match(  //
[&](connect_action a) -> connection_result {
    cerr << "connect action" << endl;
    return {state::connected{a.host, a.port}, lager::noop};
},
[&](disconnect_action a) -> connection_result {
    cerr << "disconnect action" << endl;
    return {state::idle{}, lager::noop};
},
[&](error_action a) -> connection_result {
    cerr << "disconnect action" << endl;
    return {state::error{a.ec}, lager::noop};
})(std::move(a));
}
*/

auto update_api_request(model m, api::request::requests a) -> api_result {
    return scelta::match(  //
        [&](api::request::get_some_db_data a) -> api_result {
            cerr << "net::api::request::get_some_db_data" << endl;
            return {std::move(m), lager::noop};
        },
        [&](api::request::check_healthz a) -> api_result {
            cerr << "net::api::request::check_healthz" << endl;
            return {std::move(m), lager::noop};  //
        })(std::move(a));
}

auto update_api_response(model m, api::response::responses a) -> api_result {
    return scelta::match(  //
        [&](api::response::db_data a) -> api_result {
            cerr << "net::api::response::db_data" << endl;
            return {std::move(m),
                    [data = a.data](auto&& ctx) {
                        // handle the data back in the foo module
                        //       ctx.dispatch(foo::handle_db_data_action{data});  //
                    }

            };
        },
        [&](api::response::healthz a) -> api_result {
            cerr << "net::api::response::healthz" << endl;
            return {std::move(m), lager::noop};  //
        })(std::move(a));
}

auto update_client_api(model m, api::action a) -> api_result {
    return scelta::match(  //
        [&](api::request::requests a) -> api_result {
            cerr << "net::api::request" << endl;
            return update_api_request(m, a);
        },
        [&](api::response::responses a) -> api_result {
            cerr << "net::api::response" << endl;
            return update_api_response(m, a);
        })(std::move(a));
}

auto update(model m, action a) -> result {
    return scelta::match(  //
                           //        [&](connection_action a) -> result {
                           //            auto [new_status, eff] = update(m.status, a);
                           //            m.status = new_status;
                           //            return {std::move(m), eff};
                           //        },
        [&](api::action a) -> result {
            cerr << "net:: client api action" << endl;
            return update_client_api(m, a);
        })(std::move(a));
}

}  // namespace client
}  // namespace net
