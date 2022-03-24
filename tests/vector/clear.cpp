#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(10, TYPE());
    NS_TARGET::vector<TYPE> intVec(10, 42);

    printVector(vec);
    vec.clear();
    printVector(vec);

    printVector(intVec);
    intVec.clear();
    printVector(intVec);
}
