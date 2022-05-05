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

template < class Key, class T >
void    printSize(NS_TARGET::map<Key, T> &map)
{
    std::cout << "size: " << map.size() << std::endl;
    //std::cout << "max_size: " << map.max_size() << std::endl;
}

template < class Key, class T >
void    printMap(NS_TARGET::map<Key, T> &map)
{
    typename NS_TARGET::map<Key, T>::iterator  it = map.begin();
    typename NS_TARGET::map<Key, T>::iterator  ite = map.end(); 

    for ( ; it != ite ; ++it)
        std::cout << it->second << std::endl;
    printSize(map);
}

#endif // HELPER_HPP
