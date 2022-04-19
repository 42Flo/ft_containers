#include "helper.hpp"

int main()
{
    NS_TARGET::map<char, int>   map;
    char    toInsert = 'a';

    for (unsigned int i = 0 ; i < 10 ; ++i)
        map.insert(NS_TARGET::make_pair(toInsert++, i));
    printMap(map);

    map.erase('a');
    map.erase('d');
    map.erase('b');
    printMap(map);

    //map.erase(map.begin());
    printMap(map);

    map.erase(map.begin(), map.end());
    printMap(map);
}
