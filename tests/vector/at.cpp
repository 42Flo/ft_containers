#include "helper.hpp"

int main()
{
    NS_TARGET::vector<int> vec;

    // Assigning values using at()
    for (unsigned int i = 0 ; i < vec.size() ; ++i)
        vec.at(i) = i;

    NS_TARGET::vector<int>::iterator   it = vec.begin();
    NS_TARGET::vector<int>::iterator   ite = vec.end();
    NS_TARGET::vector<int> const       vec2(it, ite);

    for (unsigned int i = 0 ; i < vec2.size() ; ++i)
        std::cout << "at " << i << ": " << vec2.at(i);
}
