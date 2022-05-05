#include "helper.hpp"

int main()
{
    NS_TARGET::set<char>    set;
    char    toInsert = 'a';

    for (unsigned int i = 0 ; i < 10 ; ++i)
        set.insert(toInsert++);
    printSet(set);

    set.erase('a');
    set.erase('b');
    set.erase('b');
    printSet(set);

    set.erase(set.begin());
    printSet(set);

    set.erase(set.begin(), set.end());
    printSet(set);
}
