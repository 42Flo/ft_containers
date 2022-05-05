#include "helper.hpp"

int main()
{
    NS_TARGET::map<char, int>   map;

    map['a'] = 10;
    map['b'] = 28;
    map['c'] = 10;

    map['a'] = 42;
    map['d'] = 42;

    std::cout << map['a'] << std::endl;
    std::cout << map['b'] << std::endl;
    std::cout << map['c'] << std::endl;
    std::cout << map['d'] << std::endl;
}
