#include "helper.hpp"
#include <list>

int main()
{
    std::list<int>  list;

    for (unsigned int i = 0 ; i < 10 ; ++i)
        list.push_back(i + 3);

    NS_TARGET::set<int>   set(list.begin(), list.end());

    printSet(set);
    set.clear();
    printSet(set);
}
