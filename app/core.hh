#pragma once
#include <lager/event_loop/boost_asio.hpp>
#include <lager/store.hpp>
#include "common/common.hh"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/thread.hpp>

#include <lager/util.hpp>

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

using app_result = std::pair<app_model, lager::effect<app_action,  //
                                                      lager::deps<boost::asio::io_context&>>>;

auto update(app_model m, app_action action) -> app_result;

inline void draw_viz(app_model prev, app_model curr) {
    //
    cerr << "draw viz" << endl;
}

class app {
   public:
    /////////////////////////////////////////////////////////////////////

    app()
        : store_{lager::make_store<app_action>(         //
              core::app_model{},                        //
              core::update,                             //
              lager::with_boost_asio_event_loop{ios_},  //
              lager::with_deps(std::ref(ios_)))},       //
          work_(ios_),                                  //
          timer_(ios_),                                 //
          socket_(ios_),                                //
          signals_(ios_) {
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
        signals_.async_wait(std::bind(&app::handle_stop, this));
    };

    void timeout_handler(const boost::system::error_code&) {
        timer_.async_wait([this](auto ec) {
            cerr << "done timer3" << endl;  //
        });
        cerr << "timeout finished" << endl;  //
    };

    auto fake_work() {
        timer_.expires_from_now(boost::posix_time::millisec(1000));

        timer_.async_wait([this](auto ec) {
            cerr << "done timer1" << endl;  //
        });

        timer_.async_wait([this](auto ec) {
            cerr << "done timer2" << endl;  //
            this->timeout_handler(ec);      //
        });

        store_.dispatch(foo::foo_b_action{});  //
        store_.dispatch(bar::bar_a_action{});  //
        store_.dispatch(baz::baz_a_action{});  //
        store_.dispatch(foo::foo_b_action{});  //
        store_.dispatch(bar::bar_b_action{});  //
        store_.dispatch(baz::baz_b_action{});  //

        store_.dispatch(baz::baz_a_action{});  //
    };

    auto run(size_t num_threads = 8) {
        watch(store_, draw_viz);

        for (size_t i = 0; i < num_threads; i++) {
            threads_.create_thread(boost::bind(&boost::asio::io_service::run, &ios_));
        }

        fake_work();
        threads_.join_all();
    }

    void handle_stop() {
        cerr << "handle_stop" << endl;
        ios_.stop();
        cerr << "done_stop" << endl;
    };

   private:
    lager::store<app_action, app_model> store_;

    boost::asio::io_service ios_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::deadline_timer timer_;
    boost::asio::io_service::work work_;
    boost::asio::signal_set signals_;
    boost::thread_group threads_;
};
}  // namespace core
