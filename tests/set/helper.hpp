#ifndef HELPER_HPP
# define HELPER_HPP

# include <iostream>

# ifdef STL
    #define NS_TARGET std
    #include <set>
# else
    #define NS_TARGET ft
    #include "../../containers/set.hpp"
# endif

template < class T, class Compare = std::less<T> >
void    printSize(NS_TARGET::set<T, Compare> &set)
{
    std::cout << "size: " << set.size() << std::endl;
}

template < class T, class Compare = std::less<T> >
void    printSet(NS_TARGET::set<T, Compare> &set)
{
    typename NS_TARGET::set<T, Compare>::iterator  it = set.begin();
    typename NS_TARGET::set<T, Compare>::iterator  ite = set.end();

    for ( ; it != ite ; ++it)
        std::cout << *it << std::endl;
    printSize(set);
}

#endif
