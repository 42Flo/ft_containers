#ifndef TEST_HPP
# define TEST_HPP

# include <iostream>

# define STL 1

# ifdef STL
    #define NS_TARGET std
    #include <algorithm>
    #include <stack>
    #include <map>
    #include <set>
# else
    #define NS_TARGET ft
    #include "stack.hpp"
    #include "map.hpp"
    //#include "set.hpp"
# endif

#endif // TEST_HPP
