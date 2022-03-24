#ifndef TEST_HPP
# define TEST_HPP

# include <iostream>

# ifdef STL
    #define NS_TARGET std
# else
    #define NS_TARGET ft
# endif

# ifndef TYPE
    #define TYPE int
# endif

#endif // TEST_HPP
