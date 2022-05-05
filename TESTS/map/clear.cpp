#include "helper.hpp"
#include <list>

int main()
{
    std::list<NS_TARGET::pair<int, char> >  list;

    for (unsigned int i = 0 ; i < 10 ; ++i)
        list.push_back(NS_TARGET::make_pair(i + 3, i + 80));

    NS_TARGET::map<int, char>   map(list.begin(), list.end());

    printMap(map);
    map.clear();
    printMap(map);
}
