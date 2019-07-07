//
// Created by laomd on 19-7-7.
//

#ifndef GFS_SEASTAR_APP_TEMPLATE_H
#define GFS_SEASTAR_APP_TEMPLATE_H

#include <seastar/core/app-template.hh>
namespace bpo = boost::program_options;

class client_template : public seastar::app_template {
public:
    client_template() {
        add_options()
            ("server", bpo::value<std::string>(), "Server address")
            ("port", bpo::value<uint16_t>()->default_value(10000), "RPC server port");;
    }
};

class server_template : public seastar::app_template {
public:
    server_template() {
        add_options()("port", bpo::value<uint16_t>()->default_value(10000), "RPC server port");
    }
};
#endif //GFS_SEASTAR_APP_TEMPLATE_H
