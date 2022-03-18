#ifndef TEST_HPP
# define TEST_HPP

# include <iostream>

# define STL 1

# ifdef STL
    #define NS_TARGET std
# else
    #define NS_TARGET ft
# endif

#endif // TEST_HPP
