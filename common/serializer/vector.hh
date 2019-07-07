#if !defined(SERIALIZER_VECTOR)
#define SERIALIZER_VECTOR

#include <vector>
#include "base_type.hh"

template <typename Output, typename T>
inline void write(serializer slz, Output& out, const std::vector<T>& v) {
    write_arithmetic_type(out, uint32_t(v.size()));
    for (const auto& s: v)
        write(slz, out, s);
}

template <typename Input, typename T>
inline std::vector<T> read(serializer slz, Input& in, rpc::type<std::vector<T>>) {
    auto size = read_arithmetic_type<uint32_t>(in);
    std::vector<T> ret;
    rpc::type<T> t;
    ret.reserve(size);
    for (int i = 0; i < size; i++)
        ret.push_back(read(slz, in, t));
    return ret;
}

#endif // SERIALIZER_VECTOR
