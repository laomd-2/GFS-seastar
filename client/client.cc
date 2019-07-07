#include <cmath>
#include <seastar/core/reactor.hh>
#include <seastar/core/app-template.hh>
#include <seastar/rpc/rpc.hh>
#include "../common/serializer/ipv4.hh"
#include "../common/serializer/sstring.hh"
#include "../common/serializer/vector.hh"
#include "../common/node.h"

using namespace seastar;
namespace bpo = boost::program_options;
using namespace std::chrono_literals;

int main(int ac, char** av) {
    app_template app;
    app.add_options()
                    ("port", bpo::value<uint16_t>()->default_value(10000), "RPC server port")
                    ("server", bpo::value<std::string>(), "Server address");
    static std::unique_ptr<client<serializer>> client_stub;

    app.run(ac, av, [&] {
        auto&& config = app.configuration();
        uint16_t port = config["port"].as<uint16_t>();
        client_stub = std::make_unique<client<serializer>>(ipv4_addr{config["server"].as<std::string>(), port}, 
                                                           rpc::client_options{});

        // lambda funtion in register_handler on client is just a interface
        auto regt = client_stub->register_handler(1, 
            [](const rpc::client_info&, uint16_t, std::vector<sstring>) {

            }
        );
        
        return regt(client_stub->get_stub(), 12333, {"laomd", "lalala"}).then([] (){ fmt::print("done.\n");}).finally([&] {
            client_stub.release();
        });
    });
    return 0;
}
