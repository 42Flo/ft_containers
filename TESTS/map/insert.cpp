#include "helper.hpp"
#include <vector>

int main()
{
    NS_TARGET::map<int, char>   map;
    NS_TARGET::map<char, std::string>   map2;

    NS_TARGET::pair<NS_TARGET::map<int, char>::iterator, bool>  ret;

    ret = map.insert(NS_TARGET::make_pair(2, 'a'));
    std::cout << (ret.first == map.begin()) << std::endl;
    std::cout << (ret.second == true) << std::endl;
    ret = map.insert(NS_TARGET::make_pair(2, 'b'));
    std::cout << (ret.second == false) << std::endl;
    map.insert(NS_TARGET::make_pair(3, 'w'));
    map.insert(NS_TARGET::make_pair(1, 'b'));
    printMap(map);

    std::vector<NS_TARGET::pair<char, std::string> > inputVec;
    inputVec.push_back(NS_TARGET::make_pair('a', "foo"));
    inputVec.push_back(NS_TARGET::make_pair('b', "foofoo"));
    inputVec.push_back(NS_TARGET::make_pair('c', "bar"));
    inputVec.push_back(NS_TARGET::make_pair('d', "barbar"));

    map2.insert(inputVec.begin(), inputVec.end());
    printMap(map2);
}
