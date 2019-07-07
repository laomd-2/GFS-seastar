#if !defined(GFS_SEASTAR_LOG)
#define GFS_SEASTAR_LOG

#include <seastar/util/log.hh>

seastar::logger make_logger() {
    return seastar::logger("lsa-api");
}

seastar::logger lsa_logger("lsa-api");
#endif // GFS_SEASTAR_LOG
