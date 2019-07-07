#if !defined(GFS_SEASTAR_EXCEPTION)
#define GFS_SEASTAR_EXCEPTION

#include <stdexcept>

class file_notfound_error : public std::logic_error {
public:
    virtual const char* what() {
        std::string file = std::logic_error::what();
        return (file + ": No such file or directory.").c_str();
    }
};

#endif // GFS_SEASTAR_EXCEPTION
