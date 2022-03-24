#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec;
    NS_TARGET::vector<std::string>  strVec(1, "foo");
    NS_TARGET::vector<int>  intVec;
    NS_TARGET::vector<int>  intVec2;
    int value = 1;

    vec.insert(vec.begin(), TYPE());
    printVector(vec);

    strVec.insert(strVec.end(), 2, "bar");
    printVector(strVec);
    
    for (unsigned int i = 0 ; i < 10 ; ++i)
        intVec.insert(intVec.begin(), value += 2);
    printVector(intVec);
    intVec2.insert(intVec2.begin(), intVec.begin(), intVec.end());
    printVector(intVec2);
}
