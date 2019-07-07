#if !defined(SERIALIZER_BASE_TYPE)
#define SERIALIZER_BASE_TYPE

#include "serializer.hh"
#include <seastar/rpc/rpc.hh>
using namespace seastar;

template <typename Output>
inline void write(serializer, Output& output, uint16_t v) { return write_arithmetic_type(output, v); }
template <typename Output>
inline void write(serializer, Output& output, int32_t v) { return write_arithmetic_type(output, v); }
template <typename Output>
inline void write(serializer, Output& output, uint32_t v) { return write_arithmetic_type(output, v); }
template <typename Output>
inline void write(serializer, Output& output, int64_t v) { return write_arithmetic_type(output, v); }
template <typename Output>
inline void write(serializer, Output& output, uint64_t v) { return write_arithmetic_type(output, v); }
template <typename Output>
inline void write(serializer, Output& output, double v) { return write_arithmetic_type(output, v); }
template <typename Input>
inline int32_t read(serializer, Input& input, rpc::type<uint16_t>) { return read_arithmetic_type<uint16_t>(input); }
template <typename Input>
inline int32_t read(serializer, Input& input, rpc::type<int32_t>) { return read_arithmetic_type<int32_t>(input); }
template <typename Input>
inline uint32_t read(serializer, Input& input, rpc::type<uint32_t>) { return read_arithmetic_type<uint32_t>(input); }
template <typename Input>
inline uint64_t read(serializer, Input& input, rpc::type<uint64_t>) { return read_arithmetic_type<uint64_t>(input); }
template <typename Input>
inline uint64_t read(serializer, Input& input, rpc::type<int64_t>) { return read_arithmetic_type<int64_t>(input); }
template <typename Input>
inline double read(serializer, Input& input, rpc::type<double>) { return read_arithmetic_type<double>(input); }

#endif // SERIALIZER_BASE_TYPE
