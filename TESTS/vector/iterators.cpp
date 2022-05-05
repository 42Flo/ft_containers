#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(8);
    NS_TARGET::vector<int> intVec(10);

    {
        NS_TARGET::vector<TYPE>::iterator   it(vec.begin());
        NS_TARGET::vector<TYPE>::iterator   ite;//TODO const iterator to fix
        ite = vec.end();

        std::cout << (it == vec.begin()) << std::endl;
        std::cout << (ite == vec.end()) << std::endl;

        std::cout << (it++ == vec.begin()) << std::endl;
        std::cout << (--ite == vec.end()) << std::endl;
    }
    {
        NS_TARGET::vector<TYPE>::iterator   it = intVec.begin();
        NS_TARGET::vector<TYPE>::iterator   ite = intVec.end();
        int value = 1;

        std::cout << ite - it << std::endl;

        // Assigning values using operator *
        for ( ; it != ite ; ++it)
            *it = (value *= 2);
        printVector(intVec);
        std::cout << it[3] << std::endl;

        it = intVec.begin();
        std::cout << *(it + 1) << std::endl;
        std::cout << *(3 + it) << std::endl;
        std::cout << *(ite - 4) << std::endl;

        std::cout << (it < ite) << std::endl;
        std::cout << (it > ite) << std::endl;
        std::cout << (ite <= ite) << std::endl;
        std::cout << (ite <= ite - 1) << std::endl;
        std::cout << (it >= it) << std::endl;
        std::cout << (it >= it + 1) << std::endl;
    }
}
