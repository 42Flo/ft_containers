#include "helper.hpp"

int main()
{
    NS_TARGET::set<int>    set;
    NS_TARGET::set<int>    set2;
    NS_TARGET::set<int>::iterator  it = set.begin();
    NS_TARGET::set<int>::iterator  it2 = set2.begin();

    for (unsigned int i = 0 ; i < 5 ; ++i)
        set.insert(i);
    printSet(set);
    for (unsigned int i = 10 ; i < 22 ; ++i)
        set2.insert(i);
    printSet(set2);

    set.swap(set2);
    printSet(set);
    printSet(set2);

    // Checking iterator validity
    std::cout << (it == set.begin()) << std::endl;
    std::cout << (it2 == set2.begin()) << std::endl;
}
