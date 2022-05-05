#include "helper.hpp"

int main()
{
    NS_TARGET::set<int>    set;

    for (int i = 0 ; i < 10 ; ++i)
        set.insert(i * 2);

    // bounds
    std::cout << *set.lower_bound(1) << std::endl;
    std::cout << *set.upper_bound(1) << std::endl;

    std::cout << *set.lower_bound(4) << std::endl;
    std::cout << *set.upper_bound(4) << std::endl;

    std::cout << *set.lower_bound(17) << std::endl;
    std::cout << *set.upper_bound(17) << std::endl;

    // find
    std::cout << *set.find(6) << std::endl;
    std::cout << *set.find(10) << std::endl;
    std::cout << (set.find(9) == set.end()) << std::endl;

    // count
    for (int i = 0 ; i < 20 ; ++i)
        std::cout << set.count(i) << std::endl;
}
