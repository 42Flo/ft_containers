#include "helper.hpp"

int main()
{
    NS_TARGET::vector<int>          vec;

    vec.push_back(1);
    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;
    vec.push_back(2);
    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;
    vec.push_back(42);
    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;
    vec.push_back(22);
    std::cout << vec.front() << std::endl;
    std::cout << vec.back() << std::endl;
}
