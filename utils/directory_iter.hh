#if !defined(GFS_SEASTAR_DIR_ITER)
#define GFS_SEASTAR_DIR_ITER

#include <vector>
#include <seastar/core/sstring.hh>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;

std::vector<seastar::sstring> scan_directory(const seastar::sstring& dir) {
    fs::path fullpath (dir.c_str());
    std::vector<seastar::sstring> ret;
    for(fs::directory_iterator end, iter(fullpath); iter != end; iter++){
        if (!fs::is_directory(*iter)) {
            ret.push_back(iter->path().string());
        }
    }
    return ret;
}

#endif // GFS_SEASTAR_DIR_ITER
