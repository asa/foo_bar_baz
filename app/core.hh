#pragma once
#include <lager/event_loop/boost_asio.hpp>
#include <lager/store.hpp>
#include "common/common.hh"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <lager/util.hpp>

#include "app/actions.hh"

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

namespace core {

auto update(app_model m, app_action action) -> app_result;

inline void draw_viz(app_model prev, app_model curr) { cerr << "draw viz" << endl; }

class app {
   public:
    /////////////////////////////////////////////////////////////////////

    app()
        : store_{lager::make_store<app_action>(        //
              core::app_model{},                       //
              core::update,                            //
              lager::with_boost_asio_event_loop{ios_}  //
              )},                                      //
          work_(ios_),                                 //
          timer_(ios_),                                //
          socket_(ios_){};

    void timeout_handler(const boost::system::error_code&) {
        timer_.async_wait([this](auto ec) {
            cerr << "done timer3" << endl;  //
        });
        cerr << "timeout finished" << endl;  //
    };

    auto run() {
        watch(store_, draw_viz);

        /*
        timer_.expires_from_now(boost::posix_time::millisec(1000));

        timer_.async_wait([this](auto ec) {
            cerr << "done timer1" << endl;  //
        });

        timer_.async_wait([this](auto ec) {
            cerr << "done timer2" << endl;  //
            this->timeout_handler(ec);      //
        });

        */

        ios_.run();
        cerr << "client.run is done" << endl;
    }

   private:
    lager::store<app_action, app_model> store_;
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::deadline_timer timer_;
    boost::asio::io_service::work work_;
};
}  // namespace core
