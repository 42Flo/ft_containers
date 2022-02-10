#ifndef MAIN_CPP
# define MAIN_CPP

# include <iostream>

# ifdef STL_EXAMPLE
    #include <aector>
    #include <algorithm>
    namespace ft = std;
# else
    #include "vector.hpp"
# endif

int	main()
{
    ft::vector<int> a;
    ft::vector<int> b(10, 21);

    for (unsigned int i = 1 ; i < 6 ; ++i)
        a.push_back(i);
    a.insert(a.end() - 3, 3, 42);
    a.erase(a.begin() + 2, a.end() - 2);

    std::cout << "vector a" << std::endl;
    for (ft::vector<int>::iterator it = a.begin() ; it != a.end() ; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;
    std::cout << "vector b" << std::endl;
    for (ft::vector<int>::iterator it = b.begin() ; it != b.end() ; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;

    std::cout << "swap called" << std::endl;
    a.swap(b);
    std::cout << std::endl;

    std::cout << "vector a" << std::endl;
    for (ft::vector<int>::iterator it = a.begin() ; it != a.end() ; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;
    std::cout << "vector b" << std::endl;
    for (ft::vector<int>::iterator it = b.begin() ; it != b.end() ; ++it)
        std::cout << *it << std::endl;

    return (0);
}

#endif // MAIN_CPP
