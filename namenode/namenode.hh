#if !defined(GFS_SEASTAR_NAMENODE)
#define GFS_SEASTAR_NAMENODE

#include <unordered_map>
#include <set>
#include <map>
#include "../common/node.h"
#include "../common/macro.h"
#include "load_balance.hh"
#include "../utils/log.hh"

namespace std {
    template <>
    struct less<ipv4_addr> {
        constexpr bool operator()(const ipv4_addr& a, const ipv4_addr& b) {
            return a.ip < b.ip || (a.ip == b.ip && a.port < b.port);
        }
    };
}

template <typename serializer, typename LB_policy=random_policy<ipv4_addr> >
class namenode : public server<serializer> {
    LB_policy _load_balancer;
    std::unordered_map<sstring, std::set<ipv4_addr>> file_datanodes;

    void add_datanode(const char* ip, uint16_t port, 
                      const std::vector<sstring>& files) {
        ipv4_addr addr{ip, port};
        for (const auto& f: files) {
            file_datanodes[f].insert(addr);
        }
    }

    void as_registry(uint32_t slot) {
        server<serializer>::register_handler(slot, 
                [this] (const rpc::client_info& info, uint16_t port,
                        std::vector<sstring> files) {
            const char* ip = inet_ntoa(info.addr.as_posix_sockaddr_in().sin_addr);
            add_datanode(ip, port, files);
            lsa_logger.info("{}:{} registered as a datanode.\n", ip, port);
            std::cout << std::endl;
        });
    }

    void as_file_searcher(uint32_t slot) {
        server<serializer>::register_handler(slot,
                [this] (sstring filename) {
            auto it = file_datanodes.find(filename);
            if (it == file_datanodes.end()) {
                lsa_logger.error("{}: No such file or directory.\n");
                return ipv4_addr(0);
            }
            else {
                ipv4_addr datanode = _load_balancer(it->second);
                lsa_logger.info("{}: load balancer return %s:%d.\n", 
                            filename.c_str(),
                            inet_ntoa(in_addr{datanode.ip}), 
                            datanode.port);
                return datanode;
            }
        });
    }
public:
    namenode(uint16_t port, rpc::server_options so, rpc::resource_limits limits)
        : server<serializer>(port, so, limits) {
        // register a datanode into namenode(a registry)
        as_registry(NAMENODE_REGISTRY_SLOT);
        // enable the client to search which datanode holds the file
        as_file_searcher(NAMENODE_FILE_SEARCH_SLOT);
    }
};

#endif // GFS_SEASTAR_NAMENODE
