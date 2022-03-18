#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(5, TYPE());
    NS_TARGET::vector<TYPE> vec2(12, TYPE());
    NS_TARGET::vector<TYPE>::iterator it = vec.begin();
    NS_TARGET::vector<TYPE>::iterator it2 = vec2.begin();

    printVector(vec);
    printVector(vec2);
    NS_TARGET::swap(vec, vec2); // Using non-member swap
    printVector(vec);
    printVector(vec2);

    // Check iterator validity
    std::cout << (it == vec.begin()) << std::endl;
    std::cout << (it2 == vec2.begin()) << std::endl;

    NS_TARGET::vector<int>  intVec(5, 1);
    NS_TARGET::vector<int>  intVec2(28, 2);

    printVector(intVec);
    printVector(intVec2);
    intVec.swap(intVec2); // Using member swap
    printVector(intVec);
    printVector(intVec2);
}
