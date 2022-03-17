#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec;
    NS_TARGET::vector<TYPE> vec2(10, TYPE());
    srand(time(NULL));

    std::cout << vec.empty() << std::endl;
    std::cout << vec2.empty() << std::endl;

    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec.resize(rand() % MAX_RAND + 1);
    printSize(vec);
    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec2.resize(rand() % MAX_RAND + 1);
    printSize(vec2);

    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec.reserve(rand() % MAX_RAND + 1);
    printSize(vec);
    for (unsigned int i = 0 ; i < 10 ; ++i)
        vec2.reserve(rand() % MAX_RAND + 1);
    printSize(vec2);
}
