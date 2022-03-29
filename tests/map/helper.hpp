#ifndef HELPER_HPP
# define HELPER_HPP

# include <iostream>

# ifdef STL
    #define NS_TARGET std
    #include <map>
# else
    #define NS_TARGET ft
    #include "../../containers/map.hpp"
# endif

template < class Key, class T, class Compare = std::less<Key> >
void    printSize(NS_TARGET::map<Key, T, Compare> &map)
{
    std::cout << "size: " << map.size() << std::endl;
    std::cout << "max_size: " << map.map_size() << std::endl;
}

template < class Key, class T, class Compare = std::less<Key> >
void    printMap(NS_TARGET::map<Key, T, Compare> &map)
{
    typename NS_TARGET::map<Key, T, Compare>::iterator  it = map.begin();
    typename NS_TARGET::map<Key, T, Compare>::iterator  ite = map.end(); 

    for ( ; it != ite ; ++it)
        std::cout << *it << std::endl;
    printSize(map);
}

#endif // HELPER_HPP
