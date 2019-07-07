#if !defined(GFS_SEASTAR_DATANODE)
#define GFS_SEASTAR_DATANODE

#include <seastar/core/future-util.hh>
#include "../common/node.h"
#include "../namenode/client.hh"
#include "../common/macro.h"
#include "../utils/log.hh"
#include "../utils/directory_iter.hh"
using namespace std::chrono_literals;
using namespace seastar;

template <typename serializer>
class datanode : public server<serializer> {
    class namenode_helper : client<serializer> {
    public:
        namenode_helper(const ipv4_addr& namenode_addr, 
                    const std::string& work_dir, uint16_t work_port)
            : client<serializer>(namenode_addr, rpc::client_options{}) {
            // lambda funtion in register_handler on client is just a interface
            auto regt = client<serializer>::register_handler(NAMENODE_REGISTRY_SLOT, 
                [](const rpc::client_info&, uint16_t, std::vector<sstring>) {}
            );
            auto heart_beat = client<serializer>::register_handler(NAMENODE_HEART_SLOT, 
                [](int) { });
            
            // register into namenode with files in work_dir
            auto files = scan_directory(work_dir);
            regt(get_stub(), files).then([&lsa_logger, &work_dir, num_files = files.size()] {
                lsa_logger.info("working on {}({} files).\n", work_dir, num_files);
            });

            // send heart beat message to namenode per second
            // to annouce that datanode is alive
            keep_doing([this, heart_beat, work_port] {
                return sleep(1s).then([this, heart_beat, work_port] {
                    heart_beat(get_stub(), work_port);
                });
            });
        }
    }
    namenode_helper _client;
    std::string _work_dir;
public:
    datanode(uint16_t port, rpc::server_options so, rpc::resource_limits limits, 
             const ipv4_addr& namenode_addr, const std::string& work_dir) 
        : _client(namenode_addr, work_dir, port), _work_dir(work_dir) {
        
    }

    sstring cat(const sstring& filename) const {
        // lock the read_lock
        // get data from file
        // unlock the read_unlock
    }

    void write(const sstring& data, const sstring& filename) {
        // lock the write_lock
        // write data to file
        // unlock the read lock
    }
};

#endif // GFS_SEASTAR_DATANODE
