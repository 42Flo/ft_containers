#include "helper.hpp"

int main()
{
    NS_TARGET::vector<TYPE> vec(4);
    NS_TARGET::vector<std::string> strVec;

    vec.push_back(TYPE());
    vec.pop_back();
    vec.push_back(TYPE());
    vec.push_back(TYPE());
    vec.pop_back();
    vec.pop_back();
    vec.push_back(TYPE());
    printVector(vec);

    strVec.push_back("foo");
    strVec.push_back("bar");
    strVec.pop_back();
    strVec.push_back("bar");
    strVec.push_back("foo");
    strVec.pop_back();
    printVector(vec);
}
