#include <seastar/core/reactor.hh>
#include <seastar/rpc/rpc.hh>
#include "../common/app_template.h"
#include "../common/serializer/ipv4.hh"
#include "../common/serializer/sstring.hh"
#include "../common/serializer/vector.hh"
#include "namenode.hh"

using namespace seastar;
namespace bpo = boost::program_options;
using namespace std::chrono_literals;


int main(int ac, char** av) {
    server_template app;
    static std::unique_ptr<namenode<serializer>> server_node;

    return app.run_deprecated(ac, av, [&] {
        engine().at_exit([&] {
            server_node.release();
            return make_ready_future();
        });

        auto&& config = app.configuration();
        uint16_t port = config["port"].as<uint16_t>();
        rpc::resource_limits limits;
        limits.bloat_factor = 1;
        limits.basic_request_size = 1;
        limits.max_memory = 10'000'000;
        rpc::server_options so;
        server_node = std::make_unique<namenode<serializer>>(port, so, limits);

        std::cout << "server on port " << port << std::endl;
    });

}
