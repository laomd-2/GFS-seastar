#if !defined(GFS_SEASTAR_LOAD_BALANCE)
#define GFS_SEASTAR_LOAD_BALANCE

#include <random>

template<typename T>
struct random_policy
{
    T operator()(const std::set<T>& v) {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0, v.size() - 1);
        int dice_roll = distribution(generator);

        auto it = v.begin();
        std::advance(it, dice_roll);
        return *it;
    }
};


#endif // GFS_SEASTAR_LOAD_BALANCE
