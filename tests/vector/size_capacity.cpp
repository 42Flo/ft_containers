#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec;
    NS_TARGET::vector<TYPE> vec2(10, TYPE());

    std::cout << vec.empty() << std::endl;
    std::cout << vec2.empty() << std::endl;

    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec.resize((i + 2) / 2);
    printSize(vec);
    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec.resize(i + 4);
    printSize(vec2);

    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec.reserve(i);
    printSize(vec);
    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec2.reserve(i * 2);
    printSize(vec2);
}
