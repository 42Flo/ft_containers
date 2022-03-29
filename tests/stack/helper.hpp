#ifndef HELPER_HPP
# define HELPER_HPP

# include <iostream>

# ifdef STL
    #define NS_TARGET std
    #include <stack>
# else
    #define NS_TARGET ft
    #include "../../containers/stack.hpp"
# endif

# ifndef TYPE
    #define TYPE int
# endif

template < class T >
void    printSize(T &st)
{
    std::cout << "size: " << st.size() << std::endl;
}

#endif // HELPER_HPP
