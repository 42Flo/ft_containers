#include "helper.hpp"
#include <vector>

int main()
{
    NS_TARGET::set<int> set;
    NS_TARGET::set<char> set2;

    NS_TARGET::pair<NS_TARGET::set<int>::iterator, bool>    ret;

    ret = set.insert(2);
    std::cout << (ret.first == set.begin()) << std::endl;
    std::cout << (ret.second == true) << std::endl;
    ret = set.insert(2);
    std::cout << (ret.second == false) << std::endl;
    set.insert(3);
    set.insert(1);
    printSet(set);

    std::vector<char>  inputVec;
    inputVec.push_back('a');
    inputVec.push_back('b');
    inputVec.push_back('c');
    inputVec.push_back('d');
    set2.insert(inputVec.begin(), inputVec.end());
    printSet(set2);
}
