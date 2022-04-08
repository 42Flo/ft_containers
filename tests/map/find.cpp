#include "helper.hpp"

int main()
{
    NS_TARGET::map<int, int>    map;
    NS_TARGET::map<std::string, std::string>    map2;

    for (unsigned int i = 0 ; i < 5 ; ++i)
        map[i] = i * 2;
    printMap(map);

    if (map.find(2) != map.end())
        std::cout << map.find(2)->second << std::endl;
    if (map.find(8) != map.end())
        std::cout << map.find(8)->second << std::endl;

    std::cout << (map.find(15) == map.end()) << std::endl;
    std::cout << (map.find(1) == map.end()) << std::endl;

    map2.insert(NS_TARGET::make_pair("one", "foo"));
    map2.insert(NS_TARGET::make_pair("two", "bar"));

    //TODO fix const conversion here
    NS_TARGET::map<std::string, std::string>::const_iterator    it = map2.find("two");

    std::cout << it->second << std::endl;
}
