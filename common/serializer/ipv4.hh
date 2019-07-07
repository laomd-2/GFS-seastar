#if !defined(SERIALIZER_IPV4)
#define SERIALIZER_IPV4

#include "base_type.hh"

template <typename Output>
inline void write(serializer, Output& out, ipv4_addr v) {
    write_arithmetic_type(out, v.ip);
    write_arithmetic_type(out, v.port);
}

template <typename Input>
inline ipv4_addr read(serializer, Input& in, rpc::type<ipv4_addr>) {
    return ipv4_addr{
        read_arithmetic_type<uint32_t>(in),
        read_arithmetic_type<uint16_t>(in)
    };
}


#endif // SERIALIZER_IPV4
