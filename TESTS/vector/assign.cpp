#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(8);
    NS_TARGET::vector<TYPE> vec2;

    NS_TARGET::vector<std::string>  strVec(10, "foo");
    NS_TARGET::vector<std::string>  strVec2;

    NS_TARGET::vector<int>  intVec(4, 42);
    NS_TARGET::vector<int>  intVec2(7);

    {
        NS_TARGET::vector<TYPE>::iterator    it = vec.begin() + 2;
        NS_TARGET::vector<TYPE>::iterator    ite = vec.end() - 2;
        vec2.assign(it, ite);
    }
    printVector(vec2);
    {
        NS_TARGET::vector<TYPE>::iterator   it = vec2.begin();
        NS_TARGET::vector<TYPE>::iterator   ite = vec2.end();
        vec.assign(it, ite);
    }
    printVector(vec);

    strVec2.push_back("bar");
    strVec2.assign(10, "foo");
    std::cout << (strVec == strVec2) << std::endl;

    intVec.assign(1, 1);
    printVector(intVec);
    intVec2.assign(7, 110);
    printVector(intVec2);
    intVec2.assign(8, 111);
    printVector(intVec2);
}
