#if !defined(GFS_NAMENODE_CLIENT)
#define GFS_NAMENODE_CLIENT

#include <seastar/core/future-util.hh>
#include "../common/node.h"
#include "../common/macro.h"
#include "../utils/log.hh"
#include "../utils/directory_iter.hh"
using namespace std::chrono_literals;

template <typename serializer>
class namenode_client : public client<serializer> {
public:
    namenode_client(const ipv4_addr& namenode_addr, rpc::client_options co, 
                    const std::string& work_dir)
        : client<serializer>(namenode_addr, co) {
        // lambda funtion in register_handler on client is just a interface
        auto regt = client<serializer>::register_handler(NAMENODE_REGISTRY_SLOT, 
            [](const rpc::client_info&, uint16_t, std::vector<sstring>) {}
        );
        
        auto files = scan_directory(work_dir);
        regt(get_stub(), files).then([&lsa_logger, &work_dir, num_files = files.size()] {
            lsa_logger.info("working on {}({} files).\n", work_dir, num_files);
        });
    }
};

#endif // GFS_NAMENODE_CLIENT
