#include "helper.hpp"

int main()
{
    NS_TARGET::map<int, std::string>    map;
    NS_TARGET::map<int, std::string>    map2;
    //NS_TARGET::map<int, std::string>::iterator  it = map.begin();
    //NS_TARGET::map<int, std::string>::iterator  it2 = map2.begin();

    map.insert(NS_TARGET::make_pair(42, "foo"));
    map.insert(NS_TARGET::make_pair(2, "bar"));

    std::cout << map[42] << std::endl;

    /*for (unsigned int i = 0 ; i < 5 ; ++i)
    {
        std::cout << "insert count: " << i << std::endl;
        map.insert(NS_TARGET::make_pair(i, "foo"));
    }
    printMap(map);
    for (unsigned int i = 10 ; i < 22 ; ++i)
        map.insert(NS_TARGET::make_pair(i, "bar"));
    printMap(map2);

    map.swap(map2);
    printMap(map);
    printMap(map2);*/

    // Checking iterator validity
    //std::cout << (it == map.begin()) << std::endl;
    //std::cout << (it2 == map2.begin()) << std::endl;
}
