#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(8, TYPE());
    NS_TARGET::vector<int>  intVec(8);

    vec.erase(vec.begin(), vec.end() - 4);
    printVector(vec);

    for (unsigned int i = 0 ; i < 8 ; ++i)
        intVec.push_back(i * 2);

    intVec.erase(intVec.begin());
    printVector(vec);
    intVec.erase(intVec.begin());
    printVector(vec);
    intVec.erase(intVec.end() - 1);
    printVector(vec);
    intVec.erase(intVec.end() - 1);
    printVector(vec);

    intVec.erase(intVec.begin() + 1);
    printVector(vec);
}
