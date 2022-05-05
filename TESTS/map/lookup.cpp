#include "helper.hpp"

int main()
{
    NS_TARGET::map<int, int>    map;

    for (int i = 0 ; i < 10 ; ++i)
        map.insert(NS_TARGET::make_pair(i * 2, i));

    // bounds
    std::cout << map.lower_bound(1)->first << std::endl;
    std::cout << map.upper_bound(1)->first << std::endl;

    std::cout << map.lower_bound(4)->first << std::endl;
    std::cout << map.upper_bound(4)->first << std::endl;

    std::cout << map.lower_bound(17)->first << std::endl;
    std::cout << map.upper_bound(17)->first << std::endl;

    // find
    std::cout << map.find(6)->first << std::endl;
    std::cout << map.find(10)->first << std::endl;
    std::cout << (map.find(9) == map.end()) << std::endl;

    // count
    for (int i = 0 ; i < 20 ; ++i)
        std::cout << map.count(i) << std::endl;
}
