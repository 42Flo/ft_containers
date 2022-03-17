#include "helper.hpp"

int main()
{
    NS_TARGET::vector<int>          vec;

    vec.push_back(1);
    vec.push_back(2);

    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;

    NS_TARGET::vector<int> const    const_vec(vec);

    std::cout << const_vec.front() << std::endl;
    std::cout << const_vec.back() << std::endl;
}
