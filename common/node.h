#if !defined(GFS_SEASTAR_NODE)
#define GFS_SEASTAR_NODE

#include <seastar/rpc/rpc.hh>
#include <seastar/net/socket_defs.hh>
using namespace seastar;

template <typename serializer>
class rpc_node : public rpc::protocol<serializer> {
public:
    rpc_node() : rpc::protocol<serializer>(serializer{}) {

    }
};

template <typename serializer>
class client : public rpc_node<serializer> {
    using stub_type = typename rpc::protocol<serializer>::client;
    stub_type client_stub;
public:
    client(const ipv4_addr& server_addr, rpc::client_options co)
        : rpc_node<serializer>(),
          client_stub(*this, co, server_addr) { }
    stub_type& get_stub() {
        return client_stub;
    }
};

template <typename serializer>
class server : public rpc_node<serializer> {
    typename rpc::protocol<serializer>::server server_stub;
public:
    server(uint16_t port, rpc::server_options so, rpc::resource_limits limits)
        : rpc_node<serializer>(),
          server_stub(*this, so, ipv4_addr{port}, limits) { }
};

#endif // GFS_SEASTAR_NODE
